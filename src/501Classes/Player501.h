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

	Player501(std::string name, float bullseyeChance, float normalHitChance, float singleHitChance,float doubleHitChance, float tripleHitChance, int startingScore, Dartboard501* board, std::mt19937* rand);
	~Player501();
	int throwDart(Target target);

	int getScore() { return currentScore; }	//Simple getter can be defined in header file, to reduce clutter in main source file
	const std::string& getName() { return name; }

	bool endTurn();		//Run at the end of someone's 3 throws, checks if their score is valid, returns if they've won

	void startNewGame();
private:
	std::string name;

	float bullseyeChance;	//Sector
	float normalHitChance;

	float singleHitChance;	//Multipliers
	float doubleHitChance;
	float tripleHitChance;

	int bullseyeCount;

	int startingScore;
	int currentScore;
	int oldScore;

	std::mt19937* rand;	//Mersenne twister, for random number generation

	Dartboard501* dartBoard;	//The dartboard players are throwing at

	int hit(Target target);	//Function for hitting a specific number, includes checks for going below 50
	
};