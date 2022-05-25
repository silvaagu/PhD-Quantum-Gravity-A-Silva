#ifndef ISOTROPYMEASUREVOLUME_HPP_INCLUDED
#define ISOTROPYMEASUREVOLUME_HPP_INCLUDED

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class IsotropyVolume : public Observable {
public:
	IsotropyVolume(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "IsotropyMeasureVolume"; } //para crear una clase de Ricci hay que darle los readios de las esferas

	void process();

private:
	std::vector<int> epsilons; // array de radios
	std::vector<bool> doneL; //array de verdaderos o falsos
	std::vector<bool> vertexL;

	std::vector<double> DirectionalVolume(Vertex::Label p1, int epsilon);
};


#endif // ISOTROPYMEASUREVOLUME_HPP_INCLUDED
