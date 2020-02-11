#include "Random.h"
#include <iostream>
#include <string>
#include <math.h>

Random::Random()
{
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//Method			:	Random
	//
	//Method parameters	:	none
	//
	//Method return		:	void
	//
	//Synopsis			:   Constructor that seeds the mersenne twister with a std::random_device
	//						
	//
	//Modifications		:
	//						Date			Developer				Notes
	//						----			---------				-----
	//						10/17/2018		J. Bordage				Initial Setup
	//																
	//
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

	generateSeed();																								
}

//Generates a seed for the mersenne twister
void Random::generateSeed()
{
	std::random_device rd;																						//Creates a random device

	rng.seed(rd());																								//Generates the mersenne twister seed using the random device
}

short Random::uniformRange(short lower, short upper)
{
	std::uniform_int_distribution<int> dN(lower, upper);

	return dN(rng);
}

//Weight is on a scale of 0 - 1, (Uniform to Heavily Biased)
short Random::WeightedRange(short lower, short upper)
{
	std::geometric_distribution<short> dN(0.9 / (upper - lower + 1));

	return dN(rng) % (upper + 1 - lower) + lower;
}

bool Random::generateBool(float probability)
{
	if (probability > 0.f && probability < 1.f)
	{
		std::uniform_int_distribution<int> dN(1, round(1 / probability));

		if (dN(rng) == 1) return true;
		else return false;
	}
	else
	{
		if (probability <= 0.f) return false;
		else return true;						//if (probability >= 1.f)
	}
}