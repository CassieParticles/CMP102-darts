#include "Dartboard301.h"

Dartboard301::Dartboard301(int bullseyeValue):bullseyeValue{bullseyeValue}
{
}

Dartboard301::~Dartboard301()
{
}

int Dartboard301::getIndex(int value)	//Linear search
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

int Dartboard301::getValue(int index)
{
	while (index > 19) { index -= 20; }	//Make sure index wraps properly
	while (index < 0) { index += 20; }
	return targets[index];
}
