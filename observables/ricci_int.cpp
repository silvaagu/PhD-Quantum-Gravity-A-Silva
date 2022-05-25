// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "ricci_int.hpp"
#include <chrono>

void RicciInt::process() {
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

double RicciInt::averageSphereDistance(Vertex::Label p1, int epsilon) {
	assert(6 * epsilon + 1 < Universe::nSlices);
    auto s1 = sphere(p1, epsilon);
	printf("eps %d s1 %d\n", epsilon, s1.size());
	int t1 = p1->time;
	std::vector<double> averageDistances = {};
	for (auto p2 : s1) {
		auto s2 = sphere(p2, epsilon);
		int t2 = p2->time;

		std::vector<int> distanceList;

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
		int distanceSum = std::accumulate(distanceList.begin(), distanceList.end(), 0);
		double averageDistance = static_cast<double>(distanceSum)/static_cast<double>(epsilon*distanceList.size());

		averageDistances.push_back(averageDistance);
	}

	double intAvgDistance = std::accumulate(averageDistances.begin(), averageDistances.end(), 0.0) / averageDistances.size();

	return intAvgDistance;
}
