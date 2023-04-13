#include "Player501.h"

//Delightful initialiser list
Player501::Player501(std::string name, float bullseyeChance, float normalHitChance,float singleHitChance, float doubleHitChance, float tripleHitChance, int startingScore, Dartboard501* dartBoard, std::mt19937* rand)
	:name{ name }, bullseyeChance{ bullseyeChance }, normalHitChance{ normalHitChance }, singleHitChance{ singleHitChance }, doubleHitChance{ doubleHitChance }, tripleHitChance{ tripleHitChance },
	startingScore{startingScore}, currentScore{startingScore}, oldScore{startingScore}, bullseyeCount{0}, rand{rand}, dartBoard{dartBoard}
{

}

Player501::~Player501()
{

}

int Player501::hit(Target target)	//Handles the player hitting a specific target, deals with score going below 50
{
	//std::cout << "Player " << name << " hit " << target.sector << " with a multiplier of " << target.mult << '\n';
	currentScore -= target.sector * target.mult;
	return currentScore;
}

int Player501::throwDart(Target target)
{
	/*	Copied from main class
	Throw 1 dart
			Create struct Target, containing the section they are aiming at, and the multiplier they are aiming at (double or treble)
			new Throw function takes in a Target argument, uses rng to randomise if they hit, and if they hit the double/treble
				Use prior random calcualtion for if they hit left or right of target
				Use second random number generated for if they hit the single/double/treble
					If they aimed for single, use hingle hit chance to determine if they do (very hgih chance of succes)
						If they don't pick double or treble randomly (equal chance)
					If they aimed for double/treble, use respective chances to determine if they do (lower chance of success)
						If they don't, they hit single (double and treble are too far away to accidentally hit the other)
				Use random to modify the Target parameter passed in, then use new Hit function, that can also hit the new target
				Return new score
	*/
	float sectorAim= static_cast<float>((*rand)()) / rand->max();
	if (target.sector == dartBoard->getInnerBullseye() || target.sector==dartBoard->getOuterBullseye())	//Handle if they are aiming for the bullseye
	{
		if (sectorAim > bullseyeChance)	//Player missed
		{
			target.sector = (*rand)() % 20 + 1;
		}
		target.mult = 1;
		return hit(target);
	}
	//Target is 1-20
	if (target.sector == 0)
	{
		//std::cout << "Player " << name << " dropped their dart to score 0, smart move!\n";
		return currentScore;
	}
	if (target.sector < 0 || target.sector>20) 
	{
		return -1; 
	}	//Quit aiming at the wall

	//Player hitting a different sector, basically same as 301
	if (sectorAim > normalHitChance)	//Hits to left or right
	{
		int change = (*rand)() % 2;
		if (change == 0) { change = -1; }
		target.sector = dartBoard->getValue(dartBoard->getIndex(target.sector) + change);	//Get value to the left or right of the current sector
	}

	float multAim = static_cast<float>((*rand)()) / rand->max();


	switch (target.mult)
	{
	case 1:
		if (multAim > singleHitChance)	//They miss the single they were aiming for
		{
			target.mult = (*rand)() % 2 + 2;	//Generate random number (either 2 or 3)
		}
		return hit(target);
	case 2:
		if (multAim > doubleHitChance)	
		{
			target.mult = 1;	
		}
		return hit(target);
	case 3:
		if (multAim > tripleHitChance)
		{
			target.mult = 1;
		}
		return hit(target);
	default:		//Aiming at an invalid multiplier
		return -1;
	}
}

bool Player501::endTurn()
{
	if (currentScore == 0)	//They won
	{
		oldScore = 0;
		return true;
	}
	if (currentScore < 4)	//Any score below 4 is either softlocked, or already invalid
	{
		//std::cout << "Score is invalid, reverting to " << oldScore<<'\n';
		currentScore = oldScore;
	}
	else					//Set the oldScore to this one, keeping progress
	{
		oldScore = currentScore;
	}

	return false;
}

void Player501::startNewGame()
{
	currentScore = startingScore;
	oldScore = startingScore;
}
