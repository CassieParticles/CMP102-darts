#include "Dartboard.h"

Dartboard::Dartboard(int bullseyeValue):bullseyeValue{bullseyeValue}
{
}

Dartboard::~Dartboard()
{
}

int Dartboard::getValue(int index)
{
	while (index > 19) { index -= 20; }	//Make sure index wraps properly
	while (index < 0) { index += 20; }
	return targets[index];
}
