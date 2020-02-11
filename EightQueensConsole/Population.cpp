#include "Population.h"
#include <iostream>
#include <string>
#include <cassert>
#include "Random.h"
#include "rlutil_fixed.h"

#define OtherGenome(i) (population[i])

extern Random rng;

//Check if the position matches a queen position, if so, return "O", if not, return "X"
std::string getTileSymbol(int x, int y, const std::vector<Queen>& queenList)
{
	//Cross-checks queen positions
	for (Queen queen : queenList)
	{
		if (queen.position.x == y && queen.position.y == x)
		{
			rlutil::setColor(rlutil::RED);
			return "O ";
		}
	}

	rlutil::setColor(rlutil::WHITE);
	return "X ";
}

// Draws a 2d symbol map indicating the positions of the queens
void drawMap(const std::vector<Queen> &queenList)
{
	int x, y;

	std::cout << "\n-------------------------------------------\n";

	for (x = 0; x < 8; x++)
	{
		for (y = 0; y < 8; y++)
		{
			std::cout << getTileSymbol(x, y, queenList);
		}

		std::cout << std::endl;
	}

	rlutil::setColor(rlutil::WHITE);
}

void Population::cleanSlate(bool keepBest)
{
	//Kills the entire population (Can also keep the best genome)
	population.erase(population.begin() + (keepBest ? 1 : 0), population.end());

	//Generates the initial population
	generatePopulation();
}

void Population::evolve(int amount)
{
	int counter = 0;
	int generation;
	int totalGenerations = 0;

	for (int i = 0; i < amount; i++)
	{
		//Generates the initial poopulation (I love this typo)
		generatePopulation();

		for (generation = 0; generation < 4000; generation++)
		{
			//std::cout << "Generation #" << generation << std::endl;

			//Checks the best genome to
			//see if the solution was found
			if (population[0].fitness == 0)
			{
				drawMap(population[0].DNA);
				totalGenerations += generation;
				std::cout << "\nSolution found after \'" << generation << "\' generations" << std::endl;
				std::cout << "\n-------------------------------------------\n";
				break;
			}

			if (counter >= 100)
			{
				//Checks if the population is stagnant (nearly entirely clonelike)
				int bestFitness = population[0].fitness;
				bool stagnant = true;

				//Checks if the fitness identical throughout the population
				for (Genome genome : population)
				{
					if (genome.fitness != bestFitness)
						stagnant = false;
				}

				//If the population is stagnant it discards
				//the population and starts over
				if (stagnant)
				{
					//std::cout << "Population Stagnant... Discarding Population" << std::endl;
					cleanSlate(true);

					rng.generateSeed();
				}

				counter -= 100;
			}
			counter++;

			//Breeds parents in order to
			//create new genetic children
			breed();

			//Mutates some of the population
			//to introduce new genomes
			mutate();

			//Destroys the genomes that
			//are the least fit
			trimUnfit();
		}

		cleanSlate(false);
	}

	if(amount > 1)
		std::cout << "\n\nAverage # of generation to find Solution: " << totalGenerations / (float)amount << std::endl;

	//printPopulation();
}

Genome Population::createGenome()
{
	//DNA Initialization
	std::vector<Queen> queenList;

	//Generates the columns for each of the chromosomes
	for (int i = 0; i < 8; i++)
	{
		queenList.push_back(Queen(queenList.size(), rng.uniformRange(0, 7)));
	}

	//Saves the genome
	Genome genome = Genome(queenList);

	return genome;
}

void Population::generatePopulation()
{
	//Variables
	Genome newGenome;
	int reseedFactor = 50;

	for (int genomeIndex = 0; genomeIndex < generationSize; genomeIndex++)
	{
		//Reseeds every 50 indexes so the Mersenne
		//Twister doesn't get too stagnant
		if (genomeIndex >= reseedFactor)
		{
			reseedFactor += 50;
			rng.generateSeed();
		}

		//Generates a genome to insert into the population
		newGenome = createGenome();
		insertByFitness(newGenome);
	}
}

void Population::insertByFitness(Genome genome)
{
	//Gets the fitness of the given genome
	getFitness(genome);

	//If nothing is on the list then insert at the beginning
	if (population.empty())
	{
		//std::cout << "Inserted at back" << std::endl;
		population.push_back(genome);
		return;
	}

	for (int genomeIndex = 0; genomeIndex < population.size(); genomeIndex++)
	{
		//Inserts the genome at the index where the fitness 
		//is better than the genome at the given index
		if (genome.fitness < OtherGenome(genomeIndex).fitness)
		{
			population.insert(population.begin() + genomeIndex, genome);
			return;
		}
	}

	//If it's the worse than all existing genomes
	population.insert(population.end(), genome);
	return;
}

std::vector<Genome> Population::crossover(Genome GenomeA, Genome GenomeB, int crossPoint)
{
	//Creates clones
	std::vector<Genome> children = { Genome(GenomeA.DNA), Genome(GenomeB.DNA) };

	for (int chromosome = crossPoint; chromosome < 8; chromosome++)
	{
		//Swaps the chromosomes of the children from the crossPoint to the end of the genome
		children[0].DNA[chromosome].position.y = GenomeB.DNA[chromosome].position.y;
		children[1].DNA[chromosome].position.y = GenomeA.DNA[chromosome].position.y;
	}

	return children;
}

void Population::breed()
{
	//Variables
	int partnerIndex1 = -1, partnerIndex2 = -1;
	int crossPoint = rng.uniformRange(1, 6);		//0 & 7 are excluded so we don't just create clones of the parents
	std::vector<Genome> children;

	for (int pair = 0; pair < pairsPerGeneration; pair++)
	{
		//Generates the partner indexes
		{
			partnerIndex1 = rng.WeightedRange(0, generationSize - 1);

			do
			{
				partnerIndex2 = rng.WeightedRange(0, generationSize - 1);
			} while (partnerIndex2 == partnerIndex1);
		}

		//Breeds (crossover) the parents to create 2 children
		for (Genome genome : crossover(population[partnerIndex1], population[partnerIndex2], crossPoint))
		{
			children.push_back(genome);
		}
	}

	//Adds the children to the general population
	for (Genome genome : children)
	{
		insertByFitness(genome);
	}
}

void Population::mutate()
{
	//Variables
	std::vector<Genome> mutants;
	Genome mutation;
	int mutationRow;
	int mutationValue;

	for (Genome genome : population)
	{
		//Rolls a probability of mutation
		if (rng.generateBool(mutationChance))
		{
			//Creates a clone to be mutated
			mutation = Genome(genome.DNA);

			//Generates the mutation
			mutationRow = rng.uniformRange(0, 7);
			mutationValue = rng.uniformRange(0, 7);

			//Mutates the genome
			mutation.DNA[mutationRow].position.y = mutationValue;

			//Saves the mutant to be later added to the population
			mutants.push_back(mutation);
		}
	}

	//Inserts the 'mutants' into the general population
	for (Genome mutatedDNA : mutants)
	{
		insertByFitness(mutatedDNA);
	}
}

// Destroys the least fit genomes in the list
void Population::trimUnfit()
{
	population.erase(population.begin() + generationSize, population.end());
}

int Population::checkChromosome(Queen chromosomes, std::vector<Queen> genome)
{
	int captures = 0;

	//Checks the entire genome
	for (Queen otherQueen : genome)
	{
		//Prevents capturing itself
		if (chromosomes.position.x != otherQueen.position.x)
		{
			//Checks captures on the same row
			if (chromosomes.position.y == otherQueen.position.y)
				captures++;
			else
			{
				//Checks captures on the diagonals
				int deltaX = abs(chromosomes.position.x - otherQueen.position.x);
				int deltaY = abs(chromosomes.position.y - otherQueen.position.y);

				//If they are diagonal regardless of their relative position
				//the absolute value of their deltas will always be identical
				if (deltaX == deltaY)
					captures++;
			}
		}
	}

	return captures;
}

void Population::getFitness(Genome &genome)
{
	//Checks if the otherGenome is unevaluated (eliminates unneccessary revaluations)
	if (genome.fitness == -1)
	{
		int fitnessRating = 0;

		//Compiles the fitness of the genome
		for (Queen queen : genome.DNA)
		{
			fitnessRating += checkChromosome(queen, genome.DNA);
		}

		//Assigns the genome's fitness datamember
		genome.fitness = fitnessRating;
	}
}


//Calculates the number of generations 
void Population::generationCounter()
{
	static int counter = 0;

	counter++;
	if (counter >= 0)
	{
		counter -= 1000;

		float averageFitness = 0;
		for (int i = 0; i < generationSize; i++)
		{
			assert(population[i].fitness != -1);

			averageFitness += population[i].fitness;
		}

		drawMap(population[0].DNA);
		std::cout << "Best Fitness: " << population[0].fitness << ", Average Fitness: " << averageFitness / generationSize << std::endl;
	}
}


//Debug
void Population::printPopulation()
{
	std::cout << "Printing Population (" << population.size() << "): " << std::endl;
	for (Genome genome : population)
	{
		std::cout << "\t\t" << "Fitness: " << genome.fitness << std::endl;
		//drawMap(genome.DNA);
	}
}