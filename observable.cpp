// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#include <fstream>
#include <vector>
#include <algorithm>
#include "observable.hpp"

std::default_random_engine Observable::rng(0);  // TODO(JorenB): seed properly

void Observable::write() {
    std::string filename = data_dir + name + "-" + identifier +extension;

	std::ifstream infile(filename);
	if (!infile.good()) {
		printf("output file deleted\n");
		printf("Creating new File\n");
		clear();
	}
	infile.close();

    std::ofstream file;
    file.open(filename, std::ios::app);

	assert(file.is_open());

	file << output << "\n";
	file.close();
}

void Observable::clear() { // basicamente lo que hace es limpiar todos los datos dentro de un archivo, si no me equivoco ????
    std::string filename = data_dir + name + "-" + identifier + extension;

    std::ofstream file;
    file.open(filename, std::ios::app);

	assert(file.is_open());

	file.close();
}

std::vector<Vertex::Label> Observable::sphere(Vertex::Label origin, int radius) { //retorna una lista de vértices pertenecientes (al borde) a una esfera centrada en el vértice origen, y con radio radius
    std::vector<Vertex::Label> thisDepth;
    std::vector<Vertex::Label> nextDepth;

	std::vector<bool> doneL;
	int vmax = 0;
	for (auto v : Universe::vertices) {
		if (v > vmax) vmax = v;
	}
	doneL.resize(vmax + 1, false);

	doneL.at(origin) = true;
    thisDepth.push_back(origin);

    std::vector<Vertex::Label> vertexList;

    for (int currentDepth = 0; currentDepth < radius; currentDepth++) { // cuenta la cantidad de pasos que hace hasta llegar al borde, se queda en uno antes, porque obtiene la esfera mirando los vecinos de los que están en el interior del borde
        for (auto v : thisDepth) {
            for (auto neighbor : Universe::vertexNeighbors[v]) {
				if (!doneL.at(neighbor)) { // si todavi no lo miro, entra
                   nextDepth.push_back(neighbor); //el proximo punto de apoyo para ver la esfera se transforma en este vertice
				   doneL.at(neighbor) = true; // como ya lo miro, lo anota como verdadero
                   if (currentDepth == radius - 1) vertexList.push_back(neighbor); //si la distancia actual es igual radio, lo guarda como un vértice de la esfera
               }
            }
        }
        thisDepth = nextDepth;
        nextDepth.clear();
    }

    return vertexList;
}


std::vector<Vertex::Label> Observable::ball(Vertex::Label origin, int radius) { //retorna una lista de vértices pertenecientes (al borde e interior) a una esfera centrada en el vértice origen, y con radio radius
    std::vector<Vertex::Label> thisDepth;
    std::vector<Vertex::Label> nextDepth;

	std::vector<bool> doneL;
	int vmax = 0;
	for (auto v : Universe::vertices) {
		if (v > vmax) vmax = v;
	}
	doneL.resize(vmax + 1, false);

	doneL.at(origin) = true;
    thisDepth.push_back(origin);

    std::vector<Vertex::Label> vertexList;

    for (int currentDepth = 0; currentDepth < radius; currentDepth++) { // cuenta la cantidad de pasos que hace hasta llegar al borde, se queda en uno antes, porque obtiene la esfera mirando los vecinos de los que están en el interior del borde
        if(currentDepth==0){
                vertexList.push_back(origin);
        }
        for (auto v : thisDepth) {
            for (auto neighbor : Universe::vertexNeighbors[v]) {
				if (!doneL.at(neighbor)) { // si todavi no lo miro, entra
                   nextDepth.push_back(neighbor); //el proximo punto de apoyo para ver la esfera se transforma en este vertice
				   doneL.at(neighbor) = true; // como ya lo miro, lo anota como verdadero
                   if (currentDepth <= radius - 1) vertexList.push_back(neighbor); //si la distancia actual es igual radio, lo guarda como un vértice de la esfera
               }
            }
        }
        thisDepth = nextDepth;
        nextDepth.clear();
    }

    return vertexList;
}

std::vector<Triangle::Label> Observable::sphereDual(Triangle::Label origin, int radius) {
	std::vector<bool> doneL;
    std::vector<Triangle::Label> thisDepth;
    std::vector<Triangle::Label> nextDepth;

	int tmax = 0;
	for (auto t : Universe::triangles) {
		if (t > tmax) tmax = t;
	}
	doneL.resize(tmax + 1, false);

	doneL.at(origin) = true;
    thisDepth.push_back(origin);

    std::vector<Triangle::Label> triangleList;

    for (int currentDepth = 0; currentDepth < radius; currentDepth++) {
        for (auto t : thisDepth) {
            for (auto neighbor : Universe::triangleNeighbors[t]) {
				if (!doneL.at(neighbor)) {
                   nextDepth.push_back(neighbor);
				   doneL.at(neighbor) = true;
                   if (currentDepth == radius-1) triangleList.push_back(neighbor);
               }
            }
        }
        thisDepth = nextDepth;
        nextDepth.clear();
    }

    return triangleList;
}

std::vector<Triangle::Label> Observable::ballDual(Triangle::Label origin, int radius) {
	std::vector<bool> doneL;
    std::vector<Triangle::Label> thisDepth;
    std::vector<Triangle::Label> nextDepth;

	int tmax = 0;
	for (auto t : Universe::triangles) {
		if (t > tmax) tmax = t;
	}
	doneL.resize(tmax + 1, false);

	doneL.at(origin) = true;
    thisDepth.push_back(origin);

    std::vector<Triangle::Label> triangleList;

    for (int currentDepth = 0; currentDepth < radius; currentDepth++) {
            if(currentDepth==0){
                triangleList.push_back(origin);
            }
        for (auto t : thisDepth) {
            for (auto neighbor : Universe::triangleNeighbors[t]) {
				if (!doneL.at(neighbor)) {
                   nextDepth.push_back(neighbor);
				   doneL.at(neighbor) = true;
                   if (currentDepth <= radius-1) triangleList.push_back(neighbor);
               }
            }
        }
        thisDepth = nextDepth;
        nextDepth.clear();
    }

    return triangleList;
}

int Observable::distance(Vertex::Label v1, Vertex::Label v2) {
    if (v1 == v2) return 0;

    std::vector<bool> doneL;
    std::vector<Vertex::Label> thisDepth;
    std::vector<Vertex::Label> nextDepth;

	int vmax = 0;
	for (auto v : Universe::vertices) {
		if (v > vmax) vmax = v;
	}
	doneL.resize(vmax + 1, false);

	doneL.at(v1) = true;
    thisDepth.push_back(v1);

    int currentDepth = 0;
    do {
        for (auto v : thisDepth) {
            for (auto neighbor : Universe::vertexNeighbors[v]) {
                if (neighbor == v2) return currentDepth + 1;

				if (!doneL.at(neighbor)) {
                    nextDepth.push_back(neighbor);
					doneL.at(neighbor) = true;
                }
            }
        }
        thisDepth = nextDepth;
        nextDepth.clear();
        currentDepth++;
    } while (thisDepth.size() > 0);
}

int Observable::distanceDual(Triangle::Label t1, Triangle::Label t2) {
    if (t1 == t2) return 0;

    std::vector<bool> doneL;
    std::vector<Triangle::Label> thisDepth;
    std::vector<Triangle::Label> nextDepth;

	int tmax = 0;
	for (auto t : Universe::triangles) {
		if (t > tmax) tmax = t;
	}
	doneL.resize(tmax + 1, false);

	doneL.at(t1) = true;
    thisDepth.push_back(t1);

    int currentDepth = 0;
    do {
        for (auto t : thisDepth) {
            for (auto neighbor : Universe::triangleNeighbors[t]) {
                if (neighbor == t2) return currentDepth + 1;

				if (!doneL.at(neighbor)) {
                    nextDepth.push_back(neighbor);
					doneL.at(neighbor) = true;
                }
            }
        }
        thisDepth = nextDepth;
        nextDepth.clear();
        currentDepth++;
    } while(thisDepth.size() > 0);
}
