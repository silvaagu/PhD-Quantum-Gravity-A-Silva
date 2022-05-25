#ifndef HAUSDORFF_DUAL_HPP_INCLUDED
#define HAUSDORFF_DUAL_HPP_INCLUDED

// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <string>
#include "../observable.hpp"
#include "../universe.hpp"

class HausdorffDual : public Observable {
    public:
        HausdorffDual(std::string id) : Observable(id) { name = "hausdorff_dual"; }

        void process();

	private:
		int max_epsilon;
};

#endif // HAUSDORFF_DUAL_HPP_INCLUDED
