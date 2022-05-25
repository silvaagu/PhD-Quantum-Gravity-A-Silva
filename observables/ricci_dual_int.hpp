#ifndef RICCI_DUAL_INT_HPP_INCLUDED
#define RICCI_DUAL_INT_HPP_INCLUDED

// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class RicciDualInt : public Observable {
public:
	RicciDualInt(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "ricci_dual_int"; }

	void process();

private:
	std::vector<int> epsilons;
	std::vector<bool> doneL;
	std::vector<bool> triangleL;

	double averageSphereDistance(Triangle::Label t1, int epsilon);
};

#endif // RICCI_DUAL_INT_HPP_INCLUDED
