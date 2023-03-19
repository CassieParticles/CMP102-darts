#pragma once

#include <iostream>
#include <random>
#include <chrono>

class Player
{
private:
	std::string name;
	float bullseyeChance;
	float normalHitChance;
	int score;
	int bullseyeCount;

	std::mt19937 rand;	//Mersenne twister, for random number generation

	int hit(int target);	//Function for hitting a specific number, includes checks for going below 50
public:
	Player(std::string name, float bullseyeChance, float normalHitChance, int startingScore);
	~Player();
	bool throwDart(int target);

	int getScore() { return score; }	//Simple getter can be defined in header file, to reduce clutter in main source file
};

