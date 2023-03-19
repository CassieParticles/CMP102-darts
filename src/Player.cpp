#include "Player.h"

Player::Player(std::string name, float bullseyeChance, float normalHitChance, int startingScore)
	:name{ name }, bullseyeChance{ bullseyeChance }, normalHitChance{ normalHitChance }, score{ startingScore }, bullseyeCount{ 0 },
	rand{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) }	//Beautiful initialiser list
{

}

Player::~Player()
{
}

bool Player::throwDart(int target)
{
	constexpr int targets[20] = { 20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5 };
	int hit;

	float aim = static_cast<float>(rand()) / rand.max();	//generate a random float between 0 and 1, 

	return false;
}
