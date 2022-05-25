#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "HomogeneityMeasureCoordinationNumber.hpp"
#include <chrono>

void HomogeneityCoordinationNumber::process() {
    std::vector<std::vector<double>> epsilonCNumberList;
	std::vector<Vertex::Label> origins;

	int vmax = 0;
	for (auto v : Universe::vertices) {
		if (v > vmax) vmax = v; //busca índice mas grande de todos los vértices en el Universo (lo hago solo una vez)
	}

	for (std::vector<int>::iterator it = epsilons.begin(); it != epsilons.end(); it++) {
        Vertex::Label vaux;
        do{
            vaux=randomVertex();
        }while(!(4 * (*it) + 1 < Universe::sliceSizes[vaux->time])); // si el tamaño del slice donde pone el origen no respeta el tamaño minimo, entonces vuelve a buscar a buscar el origen (evito problemas si la distancia es purament spacelike)
		origins.push_back(vaux); // mete un origen random de una esfera por cada radio en epsilons
	}

	for (int i = 0; i < epsilons.size(); i++) {
		int epsilon = epsilons[i];
		// printf("%d - ", epsilon);

		auto origin = origins[i];

        std::vector<double> cnumbers = CoordinationNumberBall(origin, epsilon,vmax); // calcula la distancia promedio entre puntos de una esfera con otra que se superpone, ambas con radio epsilon
        epsilonCNumberList.push_back(cnumbers); // la guarda en este array

		// printf("%f\n", averageDistance);
    }

    std::string tmp = "";
    for (auto radiusarray : epsilonCNumberList) { // va a escribir de arriba a abajo, en radio creciente
            for (double cn : radiusarray){ // para cada radio, pone uno al lado del otro los valores de los numeros de coordinacion de cada punto en la bola
                tmp += std::to_string(cn);
                tmp += " ";
            }
        tmp += "\n";
    }
    output = tmp;
}

std::vector<double> HomogeneityCoordinationNumber::CoordinationNumberBall(Vertex::Label p1, int epsilon,int vmaxx) {
    std::vector<double> cnumbers;
    std::vector<Vertex::Label> bola=ball(p1, epsilon);
    for(auto p2 : bola){
        cnumbers.push_back(Universe::vertexNeighbors[p2].size());
    }
    return cnumbers; // array with all the coordination numbers of the vertices inside a ball centered at p1 with radius epsilon
}
