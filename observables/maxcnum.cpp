// Copyright 2021 Joren Brunekreef, Daniel Nemeth and Andrzej Görlich
#include "maxcnum.hpp"

void MaxCNum::process() {
	std::string tmp = "";

	std::vector<int> maxcs(Universe::nSlices, 0);

	for (auto v : Universe::vertices) {
		if (maxcs[v->time] < Universe::vertexNeighbors.at(v).size()) maxcs[v->time] = Universe::vertexNeighbors.at(v).size();
	}

	for (auto m : maxcs) {
		tmp += std::to_string(m);
		tmp += " ";
	}
	tmp.pop_back();

	output = tmp;
}
