#include <iostream>
#include <random>
#include <chrono>

#include "Player.h"
#include "Dartboard.h"

int main()
{
	std::mt19937 rand{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };
	Dartboard board{ 50 };
	Player testPlayer{ "Jeremy",0.73f,0.9f,301,&board };

	
}