#ifndef HOMOGENEITYMEASUREQRC_HPP_INCLUDED
#define HOMOGENEITYMEASUREQRC_HPP_INCLUDED

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class HomogeneityQRC : public Observable {
public:
	HomogeneityQRC(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "HomogeneityQRC"; } //para crear una clase de Ricci hay que darle los readios de las esferas

	void process();

private:
	std::vector<int> epsilons; // array de radios
    double averageSphereDistanceForHomogeneity(Vertex::Label p1, int epsilon,int vmaxx);
	std::vector<double> QRCBall(Vertex::Label p1, int epsilon,int vmaxx);
};

#endif // HOMOGENEITYMEASUREQRC_HPP_INCLUDED
