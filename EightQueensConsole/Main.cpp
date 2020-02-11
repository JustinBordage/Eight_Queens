#include <iostream>
#include <string>
#include <stdlib.h>
#include <crtdbg.h>
#include <vector>
#include <cassert>
#include <regex>
#include "rlutil_fixed.h"
#include "Random.h"
#include "Population.h"

//memory leak detection
#define CRTDBG_MAP_ALLOC
#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif  // _DEBUG

Random rng;

int main(int argc, char *argv[])
{
	//Checks for memory Leaks
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(11705);

	//Uses a Genetic algorithm to 
	//find the solution to "8 queens"
	Population community;
	community.evolve(1);

	system("pause");

	return -1;
}

















//std::initializer_list<float> numbers



/*
}*/