#ifndef MAXCNUM_HPP_INCLUDED
#define MAXCNUM_HPP_INCLUDED


// Copyright 2021 Joren Brunekreef, Daniel Nemeth and Andrzej Görlich
#pragma once

#include "../observable.hpp"
#include "../universe.hpp"

class MaxCNum : public Observable {
public:
	MaxCNum(std::string id) : Observable(id) { name = "maxcnum"; }

	void process();

private:
	void initialize() { }
};


#endif // MAXCNUM_HPP_INCLUDED
