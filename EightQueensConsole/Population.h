#pragma once
#include "Genome.h"
#include "Queen.h"
#include <vector>


class Population
{
private:
	//Stores the list of options
	std::vector<Genome> population;
public:
	//(pairsPerGeneration < generationSize)
	unsigned int pairsPerGeneration = 20;

	//Genetic variables
	unsigned int generationSize = 100;
	float mutationChance = 0.01f;

	//Uses the genetic methods below
	//to determine the best answer
	void evolve(int amount);

	//Initialization
	Genome createGenome();
	void generatePopulation();

	//Population Management
	std::vector<Genome> crossover(Genome GenomeA, Genome GenomeB, int crossPoint);
	void breed();
	void mutate();
	void trimUnfit();

	//Fitness
	int checkChromosome(Queen chromosomes, std::vector<Queen> genome);
	void getFitness(Genome &DNA);
	void insertByFitness(Genome DNA);

	//Stagnation prevention
	void cleanSlate(bool keepBest);

	//Printing Methods
	void generationCounter();
	void printPopulation();
};
