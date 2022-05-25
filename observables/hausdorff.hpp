#ifndef HAUSDORFF_HPP_INCLUDED
#define HAUSDORFF_HPP_INCLUDED

// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <string>
#include "../observable.hpp"
#include "../universe.hpp"

class Hausdorff : public Observable {
public:
	Hausdorff(std::string id) : Observable(id) { name = "hausdorff"; }

	void process();

private:
	int max_epsilon;
	std::vector<bool> doneL;
};



#endif // HAUSDORFF_HPP_INCLUDED
