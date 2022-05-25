#ifndef RICCI_HIST_HPP_INCLUDED
#define RICCI_HIST_HPP_INCLUDED

// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class RicciHist : public Observable {
public:
	RicciHist(std::string id) : Observable(id) { name = "ricci_hist"; }

	void process();

private:
	std::vector<bool> doneL;
	std::vector<bool> vertexL;

	std::string averageSphereDistanceHist(Vertex::Label p1, int epsilon);
};


#endif // RICCI_HIST_HPP_INCLUDED
