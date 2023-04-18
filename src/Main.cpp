#include <iostream>
#include <random>
#include <chrono>

#include "301Classes/Player301.h"
#include "301Classes/Dartboard301.h"

#include "501Classes/Player501.h"
#include "501Classes/Dartboard501.h"

constexpr int startingScore = 301;

int wins[2]{};

int playGame501(Player501** playerOrder, Dartboard501* board, int* playerTurns)	//Returns 0 is player 0 won, returns 1 is player 1 won
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
	

	int cPlayerIndex = 0;
	bool playerWon = false;

	while(true)	//Will run until break is called, break is called when someone wins
	{
		Player501* currentPlayer = playerOrder[cPlayerIndex];

		//First throw======================================================================================================================
		int innerBullseyeDiff = currentPlayer->getScore() - (board->getInnerBullseye() );
		int outerBullseyeDiff = currentPlayer->getScore() - (board->getOuterBullseye() );

		if (innerBullseyeDiff < 1) { innerBullseyeDiff = 1000; }	//If player score is less then bullseye, make the bullseye very undesirable
		if (outerBullseyeDiff < 1) { outerBullseyeDiff = 1000; }

		//Try to get to get closer to a score where they can checkout
		if (currentPlayer->getScore() > 180)	//No way to checkout this turn if score is above 180
		{
			currentPlayer->throwDart({ 20,3 });	//Aim for the highest score (triple 20)
		}
		else if (currentPlayer->getScore() < 25)
		{
			currentPlayer->throwDart({ 1,1 });
		}
		else if (innerBullseyeDiff < 20)
		{
			currentPlayer->throwDart({ innerBullseyeDiff - 1,1 });
		}
		else if (outerBullseyeDiff < 20)
		{
			currentPlayer->throwDart({ outerBullseyeDiff - 1,1 });
		}
		else
		{
			currentPlayer->throwDart({ 20,3 });	//Aim for the highest score (triple 20)
		}
		//Second throw====================================================================================================================
		//Try to get score to a point where the player can checkout (bullseye first, then double)
		


		//Check the bullseye
		innerBullseyeDiff = currentPlayer->getScore() - board->getInnerBullseye();
		outerBullseyeDiff = currentPlayer->getScore() - board->getOuterBullseye();

		if (innerBullseyeDiff < 0) { innerBullseyeDiff = 1000; }	//If player score is less then bullseye, make the bullseye very undesirable
		if (outerBullseyeDiff < 0) { outerBullseyeDiff = 1000; }

		if (currentPlayer->getScore() > 180)	//No way to checkout this turn if score is above 180
		{
			currentPlayer->throwDart({ 20,3 });	//Aim for the highest score (triple 20)
		}
		else if (currentPlayer->getScore() > 110)	//Cannot get to checkout point
		{
			currentPlayer->throwDart({ 20,3 });	//Aim for the highest score (triple 20)
		}
		else if (innerBullseyeDiff < 20) { currentPlayer->throwDart({ innerBullseyeDiff,1 }); }	//If player can get to checkout with single, do so
		else if (outerBullseyeDiff < 20) { currentPlayer->throwDart({ outerBullseyeDiff,1 }); }

		else if (innerBullseyeDiff == board->getInnerBullseye() || outerBullseyeDiff == board->getInnerBullseye())	//Inner bullseye will lead to bullseye checkout
		{
			currentPlayer->throwDart({ board->getInnerBullseye(),1 });
		}
		else if (innerBullseyeDiff == board->getOuterBullseye() || outerBullseyeDiff == board->getOuterBullseye())	//Outer bullseye will lead to bullseye checkout
		{
			currentPlayer->throwDart({ board->getOuterBullseye(),1 });
		}

		else if (innerBullseyeDiff < 40 && innerBullseyeDiff % 2 == 0) { currentPlayer->throwDart({ innerBullseyeDiff / 2,2 }); }	//Double to get to bullseye checkout
		else if (outerBullseyeDiff < 40 && outerBullseyeDiff % 2 == 0) { currentPlayer->throwDart({ outerBullseyeDiff / 2,2 }); }

		else if (innerBullseyeDiff < 60 && innerBullseyeDiff % 3 == 0) { currentPlayer->throwDart({ innerBullseyeDiff / 3,3 }); }	//Triple to get to bullseye checkout
		else if (outerBullseyeDiff < 60 && outerBullseyeDiff % 3 == 0) { currentPlayer->throwDart({ outerBullseyeDiff / 3,3 }); }

		//Player cannot checkout with a bullseye, try for double
		else if (currentPlayer->getScore() < 40 && currentPlayer->getScore() % 2 == 0)	//Player score is already in double range, and even
		{
			currentPlayer->throwDart({ 2,1 });	//Aim low as possible, but even
		}
		else if (currentPlayer->getScore() < 40 && currentPlayer->getScore() % 2 == 1)	//Player score is already in double range, and odd
		{
			currentPlayer->throwDart({ 1,1 });	//Low score, has even neighbor, and aiming for double means chance odd neighbor will be even anyway
		}
		else//Any time the player could get to the "double" range, they would've been able to get to the bullseye range
		{
			currentPlayer->throwDart({ 20,3 });	//Aim to get score down faster
		}
		//Third throw======================================================================================================================
		//Checkout if possible, if you can't, keep the score above 10

		if (currentPlayer->getScore() > 180)	//No way to checkout this turn if score is above 180
		{
			currentPlayer->throwDart({ 20,3 });	//Aim for the highest score (triple 20)
		}
		else if (currentPlayer->getScore() == 50 || currentPlayer->getScore() == 25)	//Checkout by bullseye
		{
			currentPlayer->throwDart({ currentPlayer->getScore(),1 });
		}
		else if (currentPlayer->getScore() < 40 && currentPlayer->getScore() % 2 == 0)	//checkout by double
		{
			currentPlayer->throwDart({ currentPlayer->getScore() / 2, 2 });
		}//Cannot checkout if at this point
		else if (currentPlayer->getScore() < 10)	//prevent score from going below 10
		{
			currentPlayer->throwDart({ 14,1 });	//Ensures score is reset
		}
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

		bool playerWon=currentPlayer->endTurn();	//Once 3 darts are thrown, end turn, 
		
		
		if (playerWon)
		{
			//std::cout << "Player " << currentPlayer->getName() << " has won!";
			return cPlayerIndex;
		}
		
		playerTurns[cPlayerIndex]++;
		cPlayerIndex = 1 - cPlayerIndex;
		//std::cout << "Player " << currentPlayer->getName() << " ended turn with score " << currentPlayer->getScore()<<"\n\n";
	}
}

int playGame301(Player301** playerOrder, Dartboard301* board, int* playerTurns)
{
	int turnsEach[2]{};

	int currentTurn = 0;

	while (playerOrder[0]->getScore() != 0 && playerOrder[1]->getScore() != 0)	//Until someone wins
	{
		int amountToReduce = playerOrder[currentTurn]->getScore() - board->getBullseyeValue();	//Get how much the player has to score
		if (amountToReduce > board->getBullseyeValue() || amountToReduce == 0) { playerOrder[currentTurn]->throwDart(board->getBullseyeValue()); }
		else if (amountToReduce > 20) { playerOrder[currentTurn]->throwDart(20); }
		else { playerOrder[currentTurn]->throwDart(amountToReduce); }

		if (playerOrder[currentTurn]->getScore() == 0)	//Check if a player won
		{
			//std::cout << playerOrder[currentTurn]->getName() << " has won in " << turnsEach[currentTurn] << '\n';
			wins[currentTurn]++;
			return currentTurn;
		}

		playerTurns[currentTurn]++;
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

	constexpr int numberOfGames{ 10000 };

	int gameType;	//Get what type of game is played
	do
	{
		std::cout << "What type of game: ";
		std::cin >> gameType;
	} while (gameType != 301 && gameType != 501);

	char playerFirst;//Get which player should throw first
	do
	{
		std::cout << "Which player should go first, (J)oe or (S)id: ";
		std::cin >> playerFirst;
	} while (playerFirst!='S'&&playerFirst!='J');

	char custom;//Get whether the players shoudl use custom accuracies
	do
	{
		std::cout << "Should the players use custom accuracies (Y/N): ";
		std::cin >> custom;
	} while (custom != 'Y' && custom != 'N');

	int sumTurns{};
	int games{};

	Dartboard301 board301{ 50 };
	Dartboard501 board501{ 25,50 };

	float bAccJoe = .71f;	//301 & 501
	float bAccSid = .73f;
	float nAccJoe = .8f;
	float nAccSid = .8f;

	float sAccJoe = .9f;	//501 exclusive
	float sAccSid = .93f;
	float dAccJoe = .4f;
	float dAccSid = 0.41f;
	float tAccJoe = .3f;
	float tAccSid = .31f;

	if (custom == 'Y')
	{
		std::cout << "All probabilities are measured 0-1\n";

		std::cout << "What will be Joe's bullseye accuracy: ";
		std::cin >> bAccJoe;

		std::cout << "What will be Sid's bullseye accuracy: ";
		std::cin >> bAccSid;

		std::cout << "What will be Joe's sector accuracy: ";
		std::cin >> nAccJoe;

		std::cout << "What will be Sid's sector accuracy: ";
		std::cin >> nAccSid;

		if (gameType == 501)	//501 specific
		{
			std::cout << "What will be Joe's single accuracy: ";
			std::cin >> sAccJoe;

			std::cout << "What will be Sid's single accuracy: ";
			std::cin >> sAccSid;

			std::cout << "What will be Joe's double accuracy: ";
			std::cin >> dAccJoe;

			std::cout << "What will be Sid's double accuracy: ";
			std::cin >> dAccSid;

			std::cout << "What will be Joe's triple accuracy: ";
			std::cin >> tAccJoe;

			std::cout << "What will be Sid's triple accuracy: ";
			std::cin >> tAccSid;
		}
	}

	//Mersenne twister random object is large, so creating one in main and giving pointers to players is probably better
	std::mt19937 rand{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };	

	Player301 Joe301{ "Joe", bAccJoe,nAccJoe,startingScore,&board301,&rand };
	Player301 Sid301{ "Sid", bAccSid,nAccSid,startingScore,&board301,&rand };

	Player501 Joe501{ "Joe", bAccJoe, nAccJoe, sAccJoe, dAccJoe, tAccJoe, 501, &board501,&rand };
	Player501 Sid501{ "Sid", bAccSid, nAccSid, sAccSid, dAccSid, tAccSid, 501, &board501,&rand };

	Player301* playerOrder301[2];
	Player501* playerOrder501[2];

	if (playerFirst == 'S')
	{
		playerOrder301[0] = &Sid301;
		playerOrder301[1] = &Joe301;
		playerOrder501[0] = &Sid501;
		playerOrder501[1] = &Joe501;
	}
	else
	{
		playerOrder301[0] = &Joe301;
		playerOrder301[1] = &Sid301;
		playerOrder501[0] = &Joe501;
		playerOrder501[1] = &Sid501;
	}

	int numberOfWins[2]{};
	int setOutcome501[14]{};	//First 7 are player1 winning, second 7 are player2 winning
	/*
		501 game is played in sets of 13, first player to reach 7 wins the set
		14 possible outcomes
			14 item long array to count each outcome
	*/

	for (int i = 0; i < numberOfGames; i++)
	{
		int numberOfTurns[2]{};
		if (gameType == 301)
		{
			numberOfWins[playGame301(playerOrder301,&board301,numberOfTurns)]++;
			playerOrder301[0]->startNewGame();
			playerOrder301[1]->startNewGame();
		}
		else
		{
			int setWins[2]{};
			for (int i = 0; i < 13; i++)
			{
				setWins[playGame501(playerOrder501, &board501, numberOfTurns)]++;

				playerOrder501[0]->startNewGame();
				playerOrder501[1]->startNewGame();
				if (setWins[0] == 7 || setWins[1] == 7) { break; }	//If a player has won the set, break out
			}
			int setWinner;	//Get who won
			if (setWins[0] == 7) { setWinner = 0; }
			else { setWinner = 1; }

			numberOfWins[setWinner]++;

			//Get index of the outcome, if player[0] won, index is between 0-6, if player[1] won, index is between 7-13
			//Specific index in range is the losing players score
			int outcomeIndex = setWinner * 7 + setWins[1 - setWinner];	//Arithmetic to get index of which outcome, 
			setOutcome501[outcomeIndex]++;


		}
	}

	std::cout << "Player " << playerOrder301[0]->getName() << " won a total of " << numberOfWins[0] << " times!\n";
	std::cout << "Player " << playerOrder301[1]->getName() << " won a total of " << numberOfWins[1] << " times!\n";

	if (gameType == 501)	//Print out each set outcome
	{
		for (int i = 0; i < 14; i++)
		{
			int winner = i / 7;
			int losingScore = i % 7;
			int scores[2] = {};
			scores[winner] = 7;
			scores[1 - winner] = losingScore;
			std::cout << "Score [" << scores[0] << "|" << scores[1] << "] happened " << setOutcome501[i] << " times!\n";
		}
	}
	
}