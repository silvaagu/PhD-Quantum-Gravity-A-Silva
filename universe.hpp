#ifndef UNIVERSE_HPP_INCLUDED
#define UNIVERSE_HPP_INCLUDED

// Copyright 2020 Joren Brunekreef and Andrzej G?rlich
#pragma once

#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "vertex.hpp"
#include "link.hpp"
#include "triangle.hpp"
#include "pool.hpp"
#include "bag.hpp"

class Universe {
public:
	static int nSlices; // numero de slices
	static int totalVolume; // target volume
	static std::vector<int> sliceSizes; // vector con tama?o de cada slice
	static bool sphere; // si es esfera o no
	static bool imported; // si viene algo importado, o no

	static Bag<Triangle, Triangle::pool_size> trianglesAll;  // All triangles. These are candidates for the add move
	static Bag<Vertex, Vertex::pool_size> verticesFour;  // Vertices with coordination number 4. These are candidates for the delete move
	static Bag<Triangle, Triangle::pool_size> trianglesFlip;  // Triangles with a right neighbor of opposite type. These triangles are candidates for the flip move

	static void initialize();

	static void create(int n_slices,int total_volume);

	// moves
	static void insertVertex(Triangle::Label t);
	static void removeVertex(Vertex::Label v);
	enum flipSide { LEFT, RIGHT };
	static void flipLink(Vertex::Label v, flipSide side);
	static void flipLink(Triangle::Label t);

	// bag consistency
	static void updateVertexCoord(Vertex::Label v, int up, int down);
	static bool isFourVertex(Vertex::Label v);

	static void check();

	static void updateVertexData();
	static void updateLinkData();
	static void updateTriangleData();

	static void exportGeometry(std::string fname);
	static void importGeometry(std::string fname);

	static std::vector<Vertex::Label> vertices;
	static std::vector<Link::Label> links;
	static std::vector<Triangle::Label> triangles;
	static std::vector<std::vector<Vertex::Label>> vertexNeighbors;
	static std::vector<std::vector<Triangle::Label>> triangleNeighbors;

	static std::vector<std::vector<Link::Label>> vertexLinks;
	static std::vector<std::vector<Link::Label>> triangleLinks;

private:
	Universe() {}
	static std::default_random_engine rng;
};


#endif // UNIVERSE_HPP_INCLUDED
