#include <iostream>
#include <random>
#include <chrono>

#include "301Classes/Player301.h"
#include "301Classes/Dartboard301.h"

#include "501Classes/Player501.h"
#include "501Classes/Dartboard501.h"

constexpr int startingScore = 301;

int wins[2]{};

int playGame501(char playerFirst)
{
	/*
	Alternate between each player

	Player throws 3 darts, trying to get as low as they can (3 treble 20s), until they reach 180
		Have oldScore stored in Player class
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
		Use new score to find good next target
		End of 3 darts being thrown, add a EndTurn function
			checks if the new score is below 0, or a value that can't end on a double, (1,2,3)
				If it is, it sets the currentScore to the oldScore (reverts it)
				If it isn't, it sets the oldScore to the currentScore (commits it)
				If the score is 0, the player wins
			Returns boolean for if that player has won (final score is 0)
	Once they reach 180, first 2 throws are trying to score to aim towards multiples of 2, or 25/50 (pick randomly)
		In the 60-180 range, still aiming high as you can
		First throw matters less, aiming to get within 20 of the outer/inner bull is better
		Second throw sees if it can get to 50/25, if not, aims for odd/even depending on if their score is odd or even
			If score-50 is achievable (<20, <40 & multiple of 2, <60 & multiple of 3), then 50 can be reached
			If score-25 is achievable (<20, <40 & multiple of 2, <60 & multiple of 3), then 25 can be reached
		Third throw should be attempting to win, if possible
	Avoid ending up below 10, if they are below 10 on their final dart, aim high to discount set
		Aiming for single 14 is best, all neighbors are 9 or more, so guarenteed to revert
	*/
	Dartboard501 board{ 50,25 };

	Player501 Joe{ "Joe",0.71f,0.8f,0.9f,0.4f,0.3f,501,&board };
	Player501 Sid{ "Sid",0.73f,0.8f,0.93f,0.41f,0.31f,501,&board };

	Player501* playerOrder[2];	//Same setting up player order as before
	int cPlayerIndex = 0;

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

	bool playerWon = false;

	while(true)	//Will run until break is called, break is called when someone wins
	{
		Player501* currentPlayer = playerOrder[cPlayerIndex];
		for (int i = 0; i < 3; i++)	//Make first 3 throws
		{
			if (currentPlayer->getScore() > 180)	//No way to checkout this turn if score is above 180
			{
				currentPlayer->throwDart({ 20,3 });	//Aim for the highest score (triple 20)
				continue;
			}
			if (i == 0)	//1st dart, aim to get within single range of bull, if not, just aim hight
			{
				int innerBullseyeDiff = currentPlayer->getScore() - board.getInnerBullseye();
				int outerBullseyeDiff = currentPlayer->getScore() - board.getOuterBullseye();

				if (innerBullseyeDiff < 0) { innerBullseyeDiff = 99999999999999; }	//If player score is less then bullseye, make the bullseye very undesirable
				if (outerBullseyeDiff < 0) { outerBullseyeDiff = 99999999999999; }

				if (innerBullseyeDiff < 20)
				{
					currentPlayer->throwDart({ innerBullseyeDiff,1 });
				}
				else if (outerBullseyeDiff < 20)
				{
					currentPlayer->throwDart({ outerBullseyeDiff,1 });
				}
				else
				{
					currentPlayer->throwDart({ 20,3 });	//Aim for the highest score (triple 20)
				}
			}
			if (i == 1)	//2nd dart, try to get score to a point where the player can checkout (bullseye first, then double)
			{
				if (currentPlayer->getScore() > 110)	//Cannot get to checkout point
				{
					currentPlayer->throwDart({ 20,3 });	//Aim for the highest score (triple 20)
					continue;
				}
				//Check the bullseye
				int innerBullseyeDiff = currentPlayer->getScore() - board.getInnerBullseye();
				int outerBullseyeDiff = currentPlayer->getScore() - board.getOuterBullseye();

				if (innerBullseyeDiff < 0) { innerBullseyeDiff = 99999999999999; }	//If player score is less then bullseye, make the bullseye very undesirable
				if (outerBullseyeDiff < 0) { outerBullseyeDiff = 99999999999999; }

				if (innerBullseyeDiff < 20) { currentPlayer->throwDart({ innerBullseyeDiff,1 }); }	//If player can get to checkout with single, do so
				else if (outerBullseyeDiff < 20) { currentPlayer->throwDart({ outerBullseyeDiff,1 }); }

				else if (innerBullseyeDiff == board.getInnerBullseye() || outerBullseyeDiff == board.getInnerBullseye())	//Inner bullseye will lead to bullseye checkout
				{
					currentPlayer->throwDart({ board.getInnerBullseye(),1 });
				}
				else if (innerBullseyeDiff == board.getOuterBullseye() || outerBullseyeDiff == board.getOuterBullseye())	//Outer bullseye will lead to bullseye checkout
				{
					currentPlayer->throwDart({ board.getOuterBullseye(),1 });
				}

				else if (innerBullseyeDiff < 40 && innerBullseyeDiff % 2 == 0) { currentPlayer->throwDart({ innerBullseyeDiff /2,2}); }	//Double to get to bullseye checkout
				else if (outerBullseyeDiff < 40 && outerBullseyeDiff % 2 == 0) { currentPlayer->throwDart({ outerBullseyeDiff /2,2}); }

				else if (innerBullseyeDiff < 60 && innerBullseyeDiff % 3 == 0) { currentPlayer->throwDart({ innerBullseyeDiff / 3,3 }); }	//Triple to get to bullseye checkout
				else if (outerBullseyeDiff < 60 && outerBullseyeDiff % 3 == 0) { currentPlayer->throwDart({ outerBullseyeDiff / 3,3 }); }

				//Player cannot checkout with a bullseye, try for double
				else if (currentPlayer->getScore() < 40)	//Player already in "double" range, try to keep value even
				{
					currentPlayer->throwDart({ 6,2 });	//Low score, has even neighbor, and aiming for double means chance odd neighbor will be even anyway
				}
				else//Any time the player could get to the "double" range, they would've been able to get to the bullseye range
				{
					currentPlayer->throwDart({ 20,3 });	//Aim to get score down faster
				}
			}
			if (i == 2)	//3rd dart, keep score above 10, checkout if so and possible
			{
				if (currentPlayer->getScore() < 10)	//prevent score from going below 10
				{
					currentPlayer->throwDart({ 14,1 });	//Ensures score is reset
				}
				else if (currentPlayer->getScore() == 50 || currentPlayer->getScore() == 25)	//Checkout by bullseye
				{
					currentPlayer->throwDart({ currentPlayer->getScore(),1 });
				}
				else if (currentPlayer->getScore() < 40 && currentPlayer->getScore()%2==0)	//checkout by double
				{
					currentPlayer->throwDart({ currentPlayer->getScore() / 2, 2 });
				}//Cannot checkout if at this point
				else if (currentPlayer->getScore() < 20)
				{
					currentPlayer->throwDart({ 20,1 });	//Keep player score higher, to reduce chance of getting stuck
				}
				else if (currentPlayer->getScore() < 40)
				{
					currentPlayer->throwDart({ 1,1 });	//Try to reduce score as little as possible
				}
				else
				{
					currentPlayer->throwDart({ 14,1 });	//Middle score, reducing score, but not 
				}
			}
		}
		currentPlayer->endTurn();
	}

	return 0;
}

int playGame301(char playerFirst)
{
	Dartboard301 board{ 50 };

	Player301 Joe{ "Joe", 0.71f,0.8f,startingScore,&board };
	Player301 Sid{ "Sid",0.73f,0.8f,startingScore,&board };

	Player301* playerOrder[2];
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

	//constexpr int sampleSize = 100000;

	//for (games;games < sampleSize; games++)
	//{
	//	sumTurns += playGame301(playerFirst);
	//}
	//std::cout << "Mean number of turns in a game: " << sumTurns / games<<'\n';	//Get mean number of turns per game
	//std::cout << "First player won " << wins[0] << " times!" << '\n';
	//std::cout << "Second player won " << wins[1] << " times!" << '\n';

	playGame501(playerFirst);
	
}