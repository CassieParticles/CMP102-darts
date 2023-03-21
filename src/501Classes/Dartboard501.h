#pragma once
class Dartboard501
{
private:
	constexpr static int targets[20] = { 20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5 };
	int innerBullseyeValue;			//If you feel like changing the value of the bullseye
	int outerBullseyeValue;
public:
	Dartboard501(int innerBullseyeValue, int outerBullseyeValue);
	~Dartboard501();
	int getIndex(int value);
	int getValue(int index);
	int getInnerBullseye() { return innerBullseyeValue; }
	int getOuterBullseye() { return outerBullseyeValue; }
};

