#include <cstdio>
#include <cmath>
#include <time.h>
#include <array>
#include "utils.h"

class GenObject {
public:
	int id;
	double value;
	double fitness;
	int parent1ID = 0, parent2ID = 0;
	double lastMutation = 0;
	void mutate(double mutation);
};

void generateRandomPopulation();
double fitnessFunction(GenObject* obj);
void calculateFitness();
void createNewGeneration();
GenObject* crossover(GenObject* obj1, GenObject* obj2);

const double RANDOM_MIN_VALUE = -10;
const double RANDOM_MAX_VALUE = 10;
const double MIN_VALUE = RANDOM_MIN_VALUE;
const double MAX_VALUE = RANDOM_MAX_VALUE;
const int POPULATION_SIZE = 50;
const int GENERATIONS_ITERATIONS = 1000;
const int CROSSOVER_POWER = 2;
const int MUTATION_POWER = 10;
const double MAX_MUTATION_AMOUNT = 1000;

std::array<GenObject*, POPULATION_SIZE> population;


int main() {
	generateRandomPopulation();
	double prevAverage = 0;
	double averageSpeedSum = 0;
	for(int i = 0; i < GENERATIONS_ITERATIONS; i++) {
		createNewGeneration();
		calculateFitness();
		printf("Generation %d\n", i + 1);
		for(GenObject* obj: population) {
			//printf("#%d val:%.15f p1:%d p2:%d fit:%.15f mut:%f\n",
			//	obj->id, obj->value, obj->parent1ID, obj->parent2ID, obj->fitness, obj->lastMutation);
			printf("#%d val:%.15f fit:%.15f mut:%.15f\n",
				obj->id, obj->value, obj->fitness, obj->lastMutation);
		}
		double averageFitSum = 0;
		for(GenObject* obj: population) {
			averageFitSum += obj->fitness;
		}
		double average = averageFitSum / POPULATION_SIZE;
		double evolutionSpeed = average - prevAverage;
		averageSpeedSum += evolutionSpeed;
		double averageEvolutionSpeed = averageSpeedSum / (i + 1);
		printf("gen:%d top:%.15f avg:%f sp: %f avgsp: %f\n",
			i + 1, population[0]->fitness, average, evolutionSpeed, averageEvolutionSpeed);
		printf("\n");
		prevAverage = average;
	}
}

double fitnessFunction(GenObject* obj) {
	double x = obj->value;
	//return x+x*sin(x)+cos(x)*(3*x-7);
	return sin(x);
}

void calculateFitness() {
	for(GenObject* obj: population) {
		obj->fitness = fitnessFunction(obj);
	}
	std::sort(population.begin(), population.end(), [](GenObject* a, GenObject* b) {
		return a->fitness > b->fitness;
	});
}

void generateRandomPopulation() {
	printf("Generating random population\n");
	for(int i = 0; i < (int)population.size(); i++) {
		GenObject* newObject = new GenObject();
		newObject->id = i;
		newObject->value = utils::randomBetween(RANDOM_MIN_VALUE, RANDOM_MAX_VALUE);
		population[i] = newObject;
	}
}

void createNewGeneration() {
	std::array<GenObject*, POPULATION_SIZE> newPopulation;
	for(int i = 0; i < POPULATION_SIZE; i++) {
		if(i == 0) {
			GenObject* newObject = crossover(population[0], population[0]);
			newObject->id = 0;
			newPopulation[i] = newObject;
			continue;
		}
		double rand1 = pow(utils::random(), CROSSOVER_POWER);
		double rand2 = pow(utils::random(), CROSSOVER_POWER);
		double scaledRand1 = rand1 * POPULATION_SIZE;
		double scaledRand2 = rand2 * POPULATION_SIZE;
		int pick1 = int(scaledRand1);
		int pick2 = int(scaledRand2);
		GenObject* newObject = crossover(population[pick1], population[pick2]);
		newObject->id = i;
		newObject->mutate(pow(utils::random(), MUTATION_POWER) * MAX_MUTATION_AMOUNT);
		newPopulation[i] = newObject;
	}
	for(GenObject* obj: population) {
		delete obj;
	}
	population = newPopulation;
}

GenObject* crossover(GenObject* obj1, GenObject* obj2) {
	GenObject* newObject = new GenObject();
	newObject->value = (obj1->value + obj2->value) / 2.0;
	newObject->parent1ID = obj1->id;
	newObject->parent2ID = obj2->id;
	return newObject;
}

void GenObject::mutate(double mutationRate) {
	double mutation = utils::randomBetween(-mutationRate, mutationRate);
	value += mutation;
	lastMutation = mutation;
}