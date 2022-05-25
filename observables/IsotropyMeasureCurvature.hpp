#ifndef ISOTROPYMEASURECURVATURE_HPP_INCLUDED
#define ISOTROPYMEASURECURVATURE_HPP_INCLUDED


#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class IsotropyCurvature : public Observable {
public:
	IsotropyCurvature(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "IsotropyMeasureCurvature"; } //para crear una clase de Ricci hay que darle los readios de las esferas

	void process();

private:
	std::vector<int> epsilons; // array de radios
	std::vector<bool> doneL; //array de verdaderos o falsos
	std::vector<bool> vertexL;

	std::vector<double> DirectionalAverageSphereDistance(Vertex::Label p1, int epsilon,int vmaxx);
};


#endif // ISOTROPYMEASURECURVATURE_HPP_INCLUDED
