#include <iostream>
#include <random>
#include <chrono>

int main()
{
	std::mt19937 rand{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };
}