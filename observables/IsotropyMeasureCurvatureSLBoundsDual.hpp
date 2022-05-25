#ifndef ISOTROPYMEASURECURVATURESLBOUNDSDUAL_HPP_INCLUDED
#define ISOTROPYMEASURECURVATURESLBOUNDSDUAL_HPP_INCLUDED

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class IsotropyCurvatureSLBoundsDual : public Observable {
public:
	IsotropyCurvatureSLBoundsDual(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "IsotropyMeasureCurvatureSLBoundsDual"; } //para crear una clase de Ricci hay que darle los readios de las esferas

	void process();

private:
	std::vector<int> epsilons; // array de radios
	std::vector<bool> doneL; //array de verdaderos o falsos
	std::vector<bool> triangleL;

	std::vector<std::vector<double>> DirectionalAverageSphereDistanceDual(Triangle::Label p1, int epsilon,int vmaxx);
};

#endif // ISOTROPYMEASURECURVATURESLBOUNDSDUAL_HPP_INCLUDED
