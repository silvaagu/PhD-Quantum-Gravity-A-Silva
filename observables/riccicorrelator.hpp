#ifndef RICCICORRELATOR_HPP_INCLUDED
#define RICCICORRELATOR_HPP_INCLUDED

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class RicciCorrelator : public Observable {
public:
	RicciCorrelator(std::string id, std::vector<int> epsilons, std::vector<int> distances) : Observable(id), epsilons(epsilons), distances(distances)  { name = "riccicorrelator"; } //para crear una clase de Ricci hay que darle los readios de las esferas

	void process();

private:
    std::vector<int> distances; // distancias entre esferas
	std::vector<int> epsilons; // asumo que array de radios
	std::vector<bool> doneL; //array de verdaderos o falsos
	std::vector<bool> vertexL;

	double averageSphereDistance(Vertex::Label p1, int epsilon);
};

#endif // RICCICORRELATOR_HPP_INCLUDED
