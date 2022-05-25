#ifndef RICCI_DUAL_HPP_INCLUDED
#define RICCI_DUAL_HPP_INCLUDED

// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <string>
#include <vector>
#include "../observable.hpp"
#include "../universe.hpp"

class RicciDual : public Observable {
    public:
        RicciDual(std::string id, std::vector<int> epsilons) : Observable(id), epsilons(epsilons) { name = "ricci_dual"; }

        void process();

	private:
		std::vector<int> epsilons;
		std::vector<bool> doneL;
		std::vector<bool> triangleL;

        double averageSphereDistance(Triangle::Label t1, int epsilon);
};



#endif // RICCI_DUAL_HPP_INCLUDED
