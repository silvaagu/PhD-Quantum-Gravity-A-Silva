#ifndef RICCI_HPP_INCLUDED
#define RICCI_HPP_INCLUDED

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class Ricci : public Observable {
public:
	Ricci(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "ricci"; } //para crear una clase de Ricci hay que darle los readios de las esferas

	void process();

private:
	std::vector<int> epsilons; // array de radios
	std::vector<bool> doneL; //array de verdaderos o falsos
	std::vector<bool> vertexL;

	double averageSphereDistance(Vertex::Label p1, int epsilon);
};


#endif // RICCI_HPP_INCLUDED
