#pragma once
class Dartboard
{
private:
	constexpr static int targets[20] = { 20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5 };
	int bullseyeValue;			//If you feel like changing the value of the bullseye
public:
	Dartboard(int bullseyeValue);
	~Dartboard();
	int getIndex(int value);
	int getValue(int index);
	int getBullseyeValue() { return bullseyeValue; }
};

