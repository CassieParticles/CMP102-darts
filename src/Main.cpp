#include <iostream>
#include <random>
#include <chrono>

#include "Player.h"
#include "Dartboard.h"

constexpr int startingScore = 301;

int main()
{
	/*
		Initialse both players, and the dartboard

		Use player input to decide which player goes first,
		Use basic logic to decide what that player should throw at, and try to throw at it
			-If player is at 50, aim at bullseye
			-If player needs more then 50 to get to 50 points, aim at bullseye
			-If player needs more then 20, aim at 20
			-Else, aim at whatever score the player needs to get to 50
	*/
	Dartboard board{ 50 };

	Player Joe{ "Joe", 0.71f,0.8f,startingScore,&board };
	Player Sid{ "Sid",0.73f,0.8f,startingScore,&board };

	Player* playerOrder[2];

	char playerFirst;//Get which player should throw first
	do
	{
		std::cout << "Which player should go first, (J)oe or (S)id: ";
		std::cin >> playerFirst;
	} while (playerFirst!='S'&&playerFirst!='J');

	if (playerFirst == 'S')
	{
		playerOrder[0] = &Sid;
		playerOrder[1] = &Joe;
	}
	else
	{
		playerOrder[0] = &Joe;
		playerOrder[1] = &Sid;
	}

	int currentTurn = 0;

	while (playerOrder[0]->getScore() != 0 && playerOrder[1]->getScore() != 0)	//Until someone wins
	{
		int amountToReduce = playerOrder[currentTurn]->getScore() - board.getBullseyeValue();	//Get how much the player has to score
		if (amountToReduce > board.getBullseyeValue() || amountToReduce==0) { playerOrder[currentTurn]->throwDart(board.getBullseyeValue()); }
		else if (amountToReduce > 20) { playerOrder[currentTurn]->throwDart(20); }
		else { playerOrder[currentTurn]->throwDart(amountToReduce); }

		if (playerOrder[currentTurn]->getScore() == 0)	//Check if a player won
		{
			std::cout << playerOrder[currentTurn]->getName() << " has won!\n";
		}

		currentTurn = 1 - currentTurn;	//Flip it between 0 and 1
	}
	
	
}