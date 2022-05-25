#ifndef ISOTROPYMEASURECURVATUREDUAL_HPP_INCLUDED
#define ISOTROPYMEASURECURVATUREDUAL_HPP_INCLUDED

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class IsotropyCurvatureDual : public Observable {
public:
	IsotropyCurvatureDual(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "IsotropyMeasureCurvatureDual"; } //para crear una clase de Ricci hay que darle los readios de las esferas

	void process();

private:
	std::vector<int> epsilons; // array de radios


	std::vector<std::vector<double>> DirectionalAverageSphereDistanceDual(Triangle::Label p1, int epsilon,int vmaxx);
};


#endif // ISOTROPYMEASURECURVATUREDUAL_HPP_INCLUDED
