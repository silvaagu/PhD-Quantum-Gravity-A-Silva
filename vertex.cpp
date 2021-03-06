// Copyright 2020 Joren Brunekreef and Andrzej G?rlich
#include "vertex.hpp"
#include "triangle.hpp"

Triangle::Label Vertex::getTriangleLeft() { return tl; }
Triangle::Label Vertex::getTriangleRight() { return tr; }

void Vertex::setTriangleLeft(Triangle::Label t) { tl = t; }
void Vertex::setTriangleRight(Triangle::Label t) { tr = t; }
