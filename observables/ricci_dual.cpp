// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>
#include "ricci_dual.hpp"
#include <chrono>

void RicciDual::process() {
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

double RicciDual::averageSphereDistance(Triangle::Label t1, int epsilon) {
    auto s1 = sphereDual(t1, epsilon);
	std::uniform_int_distribution<> rv(0, s1.size() - 1);
    auto t2 = s1.at(rv(rng));
    auto s2 = sphereDual(t2, epsilon);

	if (s2.size() < s1.size()) {
		auto stmp = s1;
		s1 = s2;
		s2 = stmp;
	}

    std::vector<int> distanceList;

	//using std::chrono::high_resolution_clock;
    //using std::chrono::duration_cast;
    //using std::chrono::duration;
    //using std::chrono::milliseconds;

    //auto tt1 = high_resolution_clock::now();
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
	//auto tt2 = high_resolution_clock::now();

    //auto ms_int = duration_cast<milliseconds>(tt2 - tt1);
	//printf("eps: %d, t: %d\n", epsilon, ms_int);


    int distanceSum = std::accumulate(distanceList.begin(), distanceList.end(), 0);
    double averageDistance = static_cast<double>(distanceSum)/static_cast<double>(epsilon*distanceList.size());

    return averageDistance;
}
