// Copyright 2020 Joren Brunekreef and Andrzej Görlich
// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "riccih.hpp"
#include <chrono>

void RicciH::process() {
    std::vector<double> epsilonDistanceList;
	std::vector<Vertex::Label> origins;

	int vmax = 0;
	for (auto v : Universe::vertices) {
		if (v > vmax) vmax = v;
	}
	doneL.resize(vmax + 1, false);
	vertexL.resize(vmax + 1, false);

	for (std::vector<int>::iterator it = epsilons.begin(); it != epsilons.end(); it++) {
		origins.push_back(randomVertex());
	}

	for (int i = 0; i < epsilons.size(); i++) {
		int epsilon = epsilons[i];
		// printf("%d - ", epsilon);

		auto origin = origins[i];

        double averageDistance = averageSphereDistance(origin, epsilon);
        epsilonDistanceList.push_back(averageDistance);

		// printf("%f\n", averageDistance);
    }

    std::string tmp = "";
    for (double dst : epsilonDistanceList) {
        tmp += std::to_string(dst);
        tmp += " ";
    }
	tmp.pop_back();
    output = tmp;
}

double RicciH::averageSphereDistance(Vertex::Label p1, int epsilon) {
    auto s1 = sphere(p1, epsilon);
	int t1 = p1->time;

	bool possible = false;
	for (auto vv : s1) {
		if (vv->time == t1) { possible = true; break; }
	}
	if(!possible) return -1000;

	std::uniform_int_distribution<> rv(0, s1.size()-1);
	Vertex::Label p2;
	do {
		p2 = s1.at(rv(rng));
	} while (p2->time != p1->time);

    auto s2 = sphere(p2, epsilon);
	int t2 = p2->time;
	if (s2.size() < s1.size()) {
		auto stmp = s1;
		s1 = s2;
		s2 = stmp;
	}

	//int tmin = 0, tmax = 0;
	//// these bounds can be improved
	//tmax = (t1 + 5 * std::max(epsilon, 2) / 2 + 1) % Universe::nSlices;
	//tmin = (t1 - (5 * std::max(epsilon, 2) / 2 + 1) + Universe::nSlices) % Universe::nSlices;

	//assert(5 * epsilon + 1 < Universe::nSlices);

    std::vector<int> distanceList;

	///using std::chrono::high_resolution_clock;
    ///using std::chrono::duration_cast;
    ///using std::chrono::duration;
    ///using std::chrono::milliseconds;

	///auto t1 = high_resolution_clock::now();
	for (auto b : s1) {
		for (int i = 0; i < doneL.size(); i++) {
			doneL.at(i) = false;
			vertexL.at(i) = false;
		}
		for (auto v : s2) {
			vertexL.at(v) = true;
		}

		int countdown = s2.size();

		std::vector<Vertex::Label> thisDepth;
		std::vector<Vertex::Label> nextDepth;

		doneL.at(b) = true;
		thisDepth.push_back(b);

		for (int currentDepth = 0; currentDepth < 3 * epsilon + 1; currentDepth++) {
			for (auto v : thisDepth) {
				if (vertexL.at(v)) {
					distanceList.push_back(0);
					vertexL.at(v) = false;
					countdown--;
				}

				for (auto neighbor : Universe::vertexNeighbors[v]) {
					//if (neighbor->time == tmax || neighbor->time == tmin) continue;
					if (!doneL.at(neighbor)) {
						nextDepth.push_back(neighbor);
						doneL.at(neighbor) = true;

						if (vertexL.at(neighbor)) {
							distanceList.push_back(currentDepth + 1);
							vertexL.at(neighbor) = false;
							countdown--;
						}
					}
					if (countdown == 0) break;
				}
				if (countdown == 0) break;
			}
			thisDepth = nextDepth;
			nextDepth.clear();
			if (countdown == 0) break;
		}
		assert(countdown == 0);
	}
    //auto t2 = high_resolution_clock::now();

    //auto ms_int = duration_cast<milliseconds>(t2 - t1);
	//printf("eps: %d, t: %d\n", epsilon, ms_int);

    int distanceSum = std::accumulate(distanceList.begin(), distanceList.end(), 0);
    double averageDistance = static_cast<double>(distanceSum)/static_cast<double>(epsilon*distanceList.size());

    return averageDistance;
}
