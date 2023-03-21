#pragma once

#include <iostream>
#include <random>
#include <chrono>

#include "Dartboard501.h"

class Player501
{
public:
	struct Target	//What their target is, and if they are aiming for a single, a double or a treble
	{
		int sector;
		int mult;
	};

	Player501(std::string name, float bullseyeChance, float normalHitChance, int startingScore, Dartboard501* board);
	~Player501();
	bool throwDart(int target);

	int getScore() { return currentScore; }	//Simple getter can be defined in header file, to reduce clutter in main source file
	const std::string& getName() { return name; }
private:
	std::string name;
	float bullseyeChance;
	float normalHitChance;
	int bullseyeCount;

	int currentScore;
	int oldScore;

	std::mt19937 rand;	//Mersenne twister, for random number generation

	Dartboard501* dartBoard;	//The dartboard players are throwing at

	int hit(Target target);	//Function for hitting a specific number, includes checks for going below 50
};

