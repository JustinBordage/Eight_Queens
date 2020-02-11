#pragma once
#include <random>

class Random
{
public:
	std::default_random_engine generator;
	std::discrete_distribution<int> distribution{ 2,2,1,1,2,2,1,1,2,2 };

	unsigned int seed;

	//Mersenne Twister
	std::mt19937 rng;

	Random(); //Constructor that seeds the rng with a std::random_device
	void generateSeed();
	short uniformRange(short inclusiveLower, short inclusiveUpper); //Generates a number between lower and upper
	short WeightedRange(short lower, short upper);

	//Generates a bool where probability dictates the chance of rolling true (Never[0] - Always[1])
	bool generateBool(float probability);
};

//int rangeSum(short lower, short upper);