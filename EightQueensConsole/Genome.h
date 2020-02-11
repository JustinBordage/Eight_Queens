#pragma once
#include <vector>
#include "Queen.h"

class Genome
{
private:

public:
	std::vector<Queen> DNA;

	int fitness;

	Genome() : fitness(-1) {};

	Genome(std::vector<Queen> _DNA)
		: DNA(_DNA), fitness(-1) {};
};