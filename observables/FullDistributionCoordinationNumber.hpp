#ifndef FULLDISTRIBUTIONCOORDINATIONNUMBERP(Q,R)_H_INCLUDED
#define FULLDISTRIBUTIONCOORDINATIONNUMBERP(Q,R)_H_INCLUDED

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class FullDistributionCoordinationNumber : public Observable {
public:
	FullDistributionCoordinationNumber(std::string id, std::vector<int> epsilons,int VolumenUniverso,int TimeSlices) : Observable(id), epsilons(epsilons), VolumenUniverso(VolumenUniverso), TimeSlices(TimeSlices) { name = "FullDistributionCoordinationNumber"; } //para crear una clase de Ricci hay que darle los readios de las esferas

	void process();
private:
	std::vector<int> epsilons; // array de radios
	int VolumenUniverso;
	int TimeSlices;
	std::vector<double> CoordinationNumberBall(Vertex::Label p1,int epsilon,int vmaxx);
};


#endif // FULLDISTRIBUTIONCOORDINATIONNUMBERP(Q,R)_H_INCLUDED
