#ifndef RICCIH_HPP_INCLUDED
#define RICCIH_HPP_INCLUDED

// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class RicciH : public Observable {
public:
	RicciH(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "riccih"; }

	void process();

private:
	std::vector<int> epsilons;
	std::vector<bool> doneL;
	std::vector<bool> vertexL;

	double averageSphereDistance(Vertex::Label p1, int epsilon);
};


#endif // RICCIH_HPP_INCLUDED
