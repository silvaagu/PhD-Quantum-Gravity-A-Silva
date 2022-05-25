// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "ricci.hpp"
#include <chrono>

void Ricci::process() {
    std::vector<double> epsilonDistanceList;
	std::vector<Vertex::Label> origins;

	int vmax = 0;
	for (auto v : Universe::vertices) {
		if (v > vmax) vmax = v; //busca índice mas grande de todos los vértices en el Universo
	}
	doneL.resize(vmax + 1, false); // la funcion resize agrega vmax + 1 elementos con valor false al vector. Si ya tenia menos elementos que vmax+1 elementos los agrega al final, y si tenia mas, borra todos los que sobran a partir de vmax mas 1, y pone los que quiere al principio
	vertexL.resize(vmax + 1, false);

	for (std::vector<int>::iterator it = epsilons.begin(); it != epsilons.end(); it++) {
		origins.push_back(randomVertex()); // mete un origen random de una esfera por cada radio en epsilons
	}

	for (int i = 0; i < epsilons.size(); i++) {
		int epsilon = epsilons[i];
		// printf("%d - ", epsilon);

		auto origin = origins[i];

        double averageDistance = averageSphereDistance(origin, epsilon); // calcula la distancia promedio entre puntos de una esfera con otra que se superpone, ambas con radio epsilon
        epsilonDistanceList.push_back(averageDistance); // la guarda en este array

		// printf("%f\n", averageDistance);
    }

    std::string tmp = "";
    for (double dst : epsilonDistanceList) {
        tmp += std::to_string(dst); // para cada medicion, pone las distancias promedio una al lado de la otra en el archivo
        tmp += " ";
    }
	tmp.pop_back();
    output = tmp;
}

double Ricci::averageSphereDistance(Vertex::Label p1, int epsilon) {
    auto s1 = sphere(p1, epsilon); //crea esfera centrada en p1
	int t1 = p1->time; // me fijo el tiempo del centro de la esfera
	std::uniform_int_distribution<> rv(0, s1.size()-1); //genera una distribución random de los puntos que están al rededor de la esfera
    auto p2 = s1.at(rv(rng)); // toma un punto dentro de la esfera de forma random
    auto s2 = sphere(p2, epsilon); // crea una esfera al rededor de ese punto
	int t2 = p2->time; // me fijo el tiempo del centro de la siguiente esfera
	if (s2.size() < s1.size()) { // para continuar, me aseguro que la esfera mas chica sea la primera, si no es así, las intercambio para tener como s1 a la mas pequeña
		auto stmp = s1;
		s1 = s2;
		s2 = stmp;
	}

	//int tmin = 0, tmax = 0;
	//// these bounds can be improved
	//tmax = (t1 + 5 * std::max(epsilon, 2) / 2 + 1) % Universe::nSlices;
	//tmin = (t1 - (5 * std::max(epsilon, 2) / 2 + 1) + Universe::nSlices) % Universe::nSlices;

	assert(6 * epsilon + 1 < Universe::nSlices); // me fijo que el radio que uso no supere la distancia máxima permitida en una topología toroidal

    std::vector<int> distanceList; // armo una lista de distancias entre esferas

	///using std::chrono::high_resolution_clock;
    ///using std::chrono::duration_cast;
    ///using std::chrono::duration;
    ///using std::chrono::milliseconds;

	///auto t1 = high_resolution_clock::now();
	for (auto b : s1) { // para todos los vertices dentro de la esfera 1
		for (int i = 0; i < doneL.size(); i++) { // hace falsos todos los vértices de la esfera 1
			doneL.at(i) = false;
			vertexL.at(i) = false;
		}
		for (auto v : s2) { //hace verdaderos todos los vértices de la esfera 2
			vertexL.at(v) = true;
		}

		int countdown = s2.size(); //numero que va haciendo una cuenta regresiva sobre todos los vértices de la esfera que hay que mirar

		std::vector<Vertex::Label> thisDepth;
		std::vector<Vertex::Label> nextDepth;

		doneL.at(b) = true; //transforma en verdadero uno de los vértices de la esfera 1
		thisDepth.push_back(b); //lo mete en este vector

		for (int currentDepth = 0; currentDepth < 3 * epsilon + 1; currentDepth++) { // mira todo lo que está dentro de un radio menor o igual a 3 epsilon
			for (auto v : thisDepth) { //se fija para todos los vértices que hay en la esfera 1
				if (vertexL.at(v)) { // se fija si el vértice de la esfera está en la lista de fértices de la esfera 2
					distanceList.push_back(0); // le pone distancia cero
					vertexL.at(v) = false; // lo toma como vertice ya mirado
					countdown--; // reduce la cantidad de vertices a mirar en 1, porque este ya lo tuvo en cuenta
				}

				for (auto neighbor : Universe::vertexNeighbors[v]) { // ahora mira todos los vertices vecinos
					//if (neighbor->time == tmax || neighbor->time == tmin) continue;
					if (!doneL.at(neighbor)) { // se fija si ya lo miró
						nextDepth.push_back(neighbor); //lo agrega como un punto base para la siguiente iteracion
						doneL.at(neighbor) = true; // como ya lo miró, le pone verdadero

						if (vertexL.at(neighbor)) { // si uno de los vecinos del vertice pertenece a la otra esfera, entonces entra
							distanceList.push_back(currentDepth + 1); //mide la cantidad de pasos que le costó llegar a ese punto, basicamente la cantidad de links
							vertexL.at(neighbor) = false;
							countdown--;
						}
					}
					if (countdown == 0) break; // salgo si ya esta
				}
				if (countdown == 0) break; // salgo si ya esta
			}
			thisDepth = nextDepth;
			nextDepth.clear();
			if (countdown == 0) break; // salgo si ya esta
		}
		assert(countdown == 0); // chequeo que ya mire todos los puntos, sino hubo problemas
	}
    //auto t2 = high_resolution_clock::now();

    //auto ms_int = duration_cast<milliseconds>(t2 - t1);
	//printf("eps: %d, t: %d\n", epsilon, ms_int);

    int distanceSum = std::accumulate(distanceList.begin(), distanceList.end(), 0);
    double averageDistance = static_cast<double>(distanceSum)/static_cast<double>(epsilon*distanceList.size()); //distanceList.size() es la cantidad de vertices de una esfera*la cantidad de vertices de la otra. la cantidad de vértices en el medio, se obtiene simlemente multiplicando por el radio(numero de pasos hasta llegar a cada vertice. Asi se obtiene N1(epsilon)*N2(epsilon), de ambas esferas

    return averageDistance;
}
