#ifndef HOMOGENEITYMEASURECOORDINATIONNUMBER_HPP_INCLUDED
#define HOMOGENEITYMEASURECOORDINATIONNUMBER_HPP_INCLUDED

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class HomogeneityCoordinationNumber : public Observable {
public:
	HomogeneityCoordinationNumber(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "HomogeneityMeasureCoordinationNumber"; } //para crear una clase de Ricci hay que darle los readios de las esferas

	void process();

private:
	std::vector<int> epsilons; // array de radios
	std::vector<bool> doneL; //array de verdaderos o falsos
	std::vector<bool> vertexL;

	std::vector<double> CoordinationNumberBall(Vertex::Label p1, int epsilon,int vmaxx);
};

#endif // HOMOGENEITYMEASURECOORDINATIONNUMBER_HPP_INCLUDED
