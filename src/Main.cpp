#include <iostream>
#include <random>
#include <chrono>

#include "Player.h"
#include "Dartboard.h"

constexpr int startingScore = 301;

int wins[2]{};

int playGame(char playerFirst)
{
	Dartboard board{ 50 };

	Player Joe{ "Joe", 0.71f,0.8f,startingScore,&board };
	Player Sid{ "Sid",0.73f,0.8f,startingScore,&board };

	Player* playerOrder[2];
	int turnsEach[2]{};

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
		if (amountToReduce > board.getBullseyeValue() || amountToReduce == 0) { playerOrder[currentTurn]->throwDart(board.getBullseyeValue()); }
		else if (amountToReduce > 20) { playerOrder[currentTurn]->throwDart(20); }
		else { playerOrder[currentTurn]->throwDart(amountToReduce); }

		if (playerOrder[currentTurn]->getScore() == 0)	//Check if a player won
		{
			//std::cout << playerOrder[currentTurn]->getName() << " has won in " << turnsEach[currentTurn] << '\n';
			wins[currentTurn]++;
			return turnsEach[currentTurn];
		}

		turnsEach[currentTurn]++;
		currentTurn = 1 - currentTurn;	//Flip it between 0 and 1
	}
}

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
		Swap turns until someone wins
	*/
	char playerFirst;//Get which player should throw first
	do
	{
		std::cout << "Which player should go first, (J)oe or (S)id: ";
		std::cin >> playerFirst;
	} while (playerFirst!='S'&&playerFirst!='J');

	int sumTurns{};
	int games{};

	constexpr int sampleSize = 100000;

	for (games;games < sampleSize; games++)
	{
		sumTurns += playGame(playerFirst);
	}
	std::cout << "Mean number of turns in a game: " << sumTurns / games<<'\n';	//Get mean number of turns per game
	std::cout << "First player won " << wins[0] << " times!" << '\n';
	std::cout << "Second player won " << wins[1] << " times!" << '\n';
	
}