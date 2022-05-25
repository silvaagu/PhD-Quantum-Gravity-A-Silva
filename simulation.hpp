#ifndef SIMULATION_HPP_INCLUDED
#define SIMULATION_HPP_INCLUDED

// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <random>
#include <vector>
#include "universe.hpp"
#include "observable.hpp"

class Simulation {
public:
	static double lambda;
	static int seed;
	static std::string geomOut;

	static void start(int sweeps, double lambda_, int targetVolume_, int seed_, std::string geomOut);

	static void addObservable(Observable& o) {
		observables.push_back(&o);
	}

	static bool pinch;

	static std::array<int, 2> moveFreqs;
	static int attemptMove();
	static void prepare();

private:
	static std::default_random_engine rng;

	static int targetVolume;
	static double epsilon;
	static bool measuring;

	static std::vector<Observable*> observables;

	static void sweep();

	static bool moveAdd();
	static bool moveDelete();
	static bool moveFlip();


	// tuning isn't used in the current setup
	// static void tune();
	static void grow();
	static void thermalize();
};


#endif // SIMULATION_HPP_INCLUDED
