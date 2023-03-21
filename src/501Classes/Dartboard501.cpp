#include "Dartboard501.h"

Dartboard501::Dartboard501(int innerBullseyeValue, int outerBullseyeValue):innerBullseyeValue{innerBullseyeValue},outerBullseyeValue{outerBullseyeValue}
{
}

Dartboard501::~Dartboard501()
{
}

int Dartboard501::getIndex(int value)	//Linear search
{
	for (int i = 0; i < 20; i++)
	{
		if (targets[i] == value)
		{
			return i;
		}
	}
	return -1;
}

int Dartboard501::getValue(int index)
{
	while (index > 19) { index -= 20; }	//Make sure index wraps properly
	while (index < 0) { index += 20; }
	return targets[index];
}
