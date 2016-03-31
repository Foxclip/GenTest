#pragma once

#include <cstdlib>
#include <random>

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<double> dist(0.0, 1.0);

namespace utils {

	double random() {
		//return (double)rand()/(double)RAND_MAX;
		return dist(mt);
	}

	double randomBetween(double min, double max) {
		return random() * (max - min) + min;
	}

}