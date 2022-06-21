#ifndef HOMOGENEITYMEASUREHAUSDORFDIMENSION_HPP_INCLUDED
#define HOMOGENEITYMEASUREHAUSDORFDIMENSION_HPP_INCLUDED

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class HomogeneityHausdorffDimension : public Observable {
public:
	HomogeneityHausdorffDimension(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "HomogeneityMeasureHausdorffDimension"; } //para crear una clase de Ricci hay que darle los readios de las esferas

	void process();

private:
	std::vector<int> epsilons; // array de radios
	std::vector<std::vector<double>> HausdorffDimensionSphere(Vertex::Label p1, int epsilon,int vmaxx);
};

#endif // HOMOGENEITYMEASUREHAUSDORFDIMENSION_HPP_INCLUDED
