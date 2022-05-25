#ifndef RICCIV_HPP_INCLUDED
#define RICCIV_HPP_INCLUDED

// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class RicciV : public Observable {
public:
	RicciV(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "ricciv"; }

	void process();

private:
	std::vector<int> epsilons;
	std::vector<bool> doneL;
	std::vector<bool> vertexL;

	double averageSphereDistance(Vertex::Label p1, int epsilon);
};


#endif // RICCIV_HPP_INCLUDED
