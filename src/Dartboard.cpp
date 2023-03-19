#include "Dartboard.h"

Dartboard::Dartboard(int bullseyeValue):bullseyeValue{bullseyeValue}
{
}

Dartboard::~Dartboard()
{
}

int Dartboard::getIndex(int value)	//Linear search
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

int Dartboard::getValue(int index)
{
	while (index > 19) { index -= 20; }	//Make sure index wraps properly
	while (index < 0) { index += 20; }
	return targets[index];
}
