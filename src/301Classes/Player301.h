#pragma once

#include <iostream>
#include <random>
#include <chrono>

#include "Dartboard301.h"

class Player301
{
private:
	std::string name;
	float bullseyeChance;
	float normalHitChance;
	int score;
	int bullseyeCount;

	std::mt19937 rand;	//Mersenne twister, for random number generation

	Dartboard301* dartBoard;	//The dartboard players are throwing at

	int hit(int target);	//Function for hitting a specific number, includes checks for going below 50
public:
	Player301(std::string name, float bullseyeChance, float normalHitChance, int startingScore, Dartboard301* board);
	~Player301();
	bool throwDart(int target);

	int getScore() { return score; }	//Simple getter can be defined in header file, to reduce clutter in main source file
	const std::string& getName() { return name; }
};

