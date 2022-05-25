#ifndef VERTEX_HPP_INCLUDED
#define VERTEX_HPP_INCLUDED
#pragma once
#include "pool.hpp"

class Triangle;
class Vertex : public Pool<Vertex> {
public:
	static const unsigned pool_size = 10000000;
	int time;  // Slice number

	Pool<Triangle>::Label getTriangleLeft();
	Pool<Triangle>::Label getTriangleRight();

	void setTriangleLeft(Pool<Triangle>::Label t);
	void setTriangleRight(Pool<Triangle>::Label t);

private:
	Pool<Triangle>::Label tl, tr;  // triangles
};

#endif // VERTEX_HPP_INCLUDED
