#include "Player501.h"

//Delightful initialiser list
Player501::Player501(std::string name, float bullseyeChance, float normalHitChance,float singleHitChance, float doubleHitChance, float tripleHitChance, int startingScore, Dartboard501* dartBoard)
	:name{ name }, bullseyeChance{ bullseyeChance }, normalHitChance{ normalHitChance }, singleHitChance{ singleHitChance }, doubleHitChance{ doubleHitChance }, tripleHitChance{ tripleHitChance },
	currentScore{startingScore}, oldScore{startingScore}, bullseyeCount{0},	rand{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) }, dartBoard{dartBoard}	
{

}

Player501::~Player501()
{

}

int Player501::hit(Target target)	//Handles the player hitting a specific target, deals with score going below 50
{
	std::cout << "Player " << name << " hit " << target.sector << " with a multiplier of " << target.mult << '\n';
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
	float sectorAim= static_cast<float>(rand()) / rand.max();
	if (target.sector == dartBoard->getInnerBullseye() || target.sector==dartBoard->getOuterBullseye())	//Handle if they are aiming for the bullseye
	{
		if (sectorAim > bullseyeChance)	//Player missed
		{
			target.sector = rand() % 20 + 1;
		}
		target.mult = 1;
		return hit(target);
	}
	//Target is 1-20
	if (target.sector < 1 || target.sector>20) { return -1; }	//Quit aiming at the wall

	//Player hitting a different sector, basically same as 301
	if (sectorAim > normalHitChance)	//Hits to left or right
	{
		int change = rand() % 2;
		if (change == 0) { change = -1; }
		target.sector = dartBoard->getValue(dartBoard->getIndex(target.sector) + change);	//Get value to the left or right of the current sector
	}

	float multAim = static_cast<float>(rand()) / rand.max();


	switch (target.mult)
	{
	case 1:
		if (multAim > singleHitChance)	//They miss the single they were aiming for
		{
			target.mult = rand() % 2 + 2;	//Generate random number (either 2 or 3)
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

#if 0	//Do not compile this block of code
bool Player501::throwDart(int target)
{
	if (dartBoard == nullptr) { return false; }	//If there isn't a dartboard, stop player from throwing a dart at the wall and causing memory issues

	float aim = static_cast<float>(rand()) / rand.max();	//generate a random float between 0 and 1, for checking if they hit the intended targer

	if (target==dartBoard->getBullseyeValue())
	{
		if (aim > bullseyeChance)	//If the player misses, change what their target is
		{
			target = rand() % 20 + 1;	//Makes target random number between 1 and 20
		}
		hit(target);
		return score == 0;		//If player has won, return true
	}
	//Target must not be 50
	if (target < 1 || target>20) { return false; }	//If they aim at a non-board number, just exit

	if (aim > normalHitChance) //If they player misses, change their target
	{
		int index = dartBoard->getIndex(target);

		int change = rand() % 2;	//0 means the index before, 1 means the index after
		if (change == 0) { change--; }

		target = dartBoard->getValue(index + change);	//Get the place the player hits, allowing it to wrap around in the funnction
	}

	hit(target);

	return false;	//Cannot win on a non-bullseye
}
#endif