#ifndef ISOTROPYMEASURECURVATURESLBOUNDS_HPP_INCLUDED
#define ISOTROPYMEASURECURVATURESLBOUNDS_HPP_INCLUDED

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class IsotropyCurvatureSLBounds : public Observable {
public:
	IsotropyCurvatureSLBounds(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "IsotropyMeasureCurvatureSLBounds"; } //para crear una clase de Ricci hay que darle los readios de las esferas

	void process();

private:
	std::vector<int> epsilons; // array de radios
	std::vector<bool> doneL; //array de verdaderos o falsos
	std::vector<bool> vertexL;

	std::vector<std::vector<double>> DirectionalAverageSphereDistance(Vertex::Label p1, int epsilon,int vmaxx);
};


#endif // ISOTROPYMEASURECURVATURESLBOUNDS_HPP_INCLUDED
