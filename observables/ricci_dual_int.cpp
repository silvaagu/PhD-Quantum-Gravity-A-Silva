// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>
#include "ricci_dual_int.hpp"
#include <chrono>

void RicciDualInt::process() {
    std::vector<double> epsilonDistanceList;
	std::vector<Triangle::Label> origins;

	int tmax = 0;
	for (auto t : Universe::triangles) {
		if (t > tmax) tmax = t;
	}
	doneL.resize(tmax + 1, false);
	triangleL.resize(tmax + 1, false);

	for (std::vector<int>::iterator it = epsilons.begin(); it != epsilons.end(); it++) {
		origins.push_back(Universe::trianglesAll.pick());
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

double RicciDualInt::averageSphereDistance(Triangle::Label t1, int epsilon) {
    auto s1 = sphereDual(t1, epsilon);
	printf("eps %d s1 %d\n", epsilon, s1.size());

	std::vector<double> averageDistances = {};
	for (auto t2 : s1) {
		auto s2 = sphereDual(t2, epsilon);

		std::vector<int> distanceList;

		for (auto b : s1) {
			for (int i = 0; i < doneL.size(); i++) {
				doneL.at(i) = false;
				triangleL.at(i) = false;
			}
			for (auto t : s2) {
				triangleL.at(t) = true;
			}

			int countdown = s2.size();

			std::vector<Triangle::Label> thisDepth;
			std::vector<Triangle::Label> nextDepth;

			doneL.at(b) = true;
			thisDepth.push_back(b);

			for (int currentDepth = 0; currentDepth < 3 * epsilon + 1; currentDepth++) {
				for (auto v : thisDepth) {
					if (triangleL.at(v)) {
						distanceList.push_back(0);
						triangleL.at(v) = false;
						countdown--;
					}

					for (auto neighbor : Universe::triangleNeighbors[v]) {
						if (!doneL.at(neighbor)) {
							nextDepth.push_back(neighbor);
							doneL.at(neighbor) = true;

							if (triangleL.at(neighbor)) {
								distanceList.push_back(currentDepth + 1);
								triangleL.at(neighbor) = false;
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
