#include "Player.h"

//Beautiful initialiser list
Player::Player(std::string name, float bullseyeChance, float normalHitChance, int startingScore, Dartboard* dartBoard)
	:name{ name }, bullseyeChance{ bullseyeChance }, normalHitChance{ normalHitChance }, score{ startingScore }, bullseyeCount{ 0 },
	rand{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) }, dartBoard{dartBoard}	
{

}

Player::~Player()
{
}

int Player::hit(int target)	//Handles the player hitting a specific target, deals with score going below 50
{
	std::cout << name <<" hits " << target << '\n';
	if (score - target == 0) //Must have hit a bullseye
	{ 
		bullseyeCount++;
		return score -= target; //Subtracts the target from the score, (will always be 0), then returns that 0
	}
	if (score - target < dartBoard->getBullseyeValue()) //Non-bullseye takes it below 50, invalid and disregarded
	{
		std::cout << "Cannot go below "<< dartBoard->getBullseyeValue()<<", must end on a bullseye\n";
		return score;
	}	
	return score -= target;
}

bool Player::throwDart(int target)
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
	if (target < 0 || target>20) { return false; }	//If they aim at a non-board number, just exit

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
