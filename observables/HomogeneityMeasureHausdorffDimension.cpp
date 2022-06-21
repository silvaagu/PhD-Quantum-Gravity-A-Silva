#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "HomogeneityMeasureHausdorffDimension.hpp"
#include <chrono>

void HomogeneityHausdorffDimension::process() {
    std::vector<std::vector<std::vector<double>>> epsilonspheresList;
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

        std::vector<std::vector<double>> esferasenbola = HausdorffDimensionSphere(origin, epsilon,vmax); // calcula la distancia promedio entre puntos de una esfera con otra que se superpone, ambas con radio epsilon
        epsilonspheresList.push_back(esferasenbola); // la guarda en este array
		// printf("%f\n", averageDistance);
    }

    std::string tmp = "";
    for (auto radiusarray : epsilonspheresList) { // va a escribir de arriba a abajo, en radio creciente
            for (auto cn : radiusarray){ // para cada radio, pone uno al lado del otro un array de los tamanios de esferas hasta radio epsilon max, centradas en cada punto en la bola
                    for(double d : cn){
                        tmp += std::to_string(d);
                        tmp += " ";
                    }
            }
        tmp += "\n";
    }
    output = tmp;
}

std::vector<std::vector<double>> HomogeneityHausdorffDimension::HausdorffDimensionSphere(Vertex::Label p1, int epsilon,int vmaxx) {
    std::vector<std::vector<double>> successivespheres;
    std::vector<Vertex::Label> bola=ball(p1, epsilon);
    for(auto p2 : bola){
        std::vector<double> esferas;
        for (std::vector<int>::iterator it = epsilons.begin(); it != epsilons.end(); it++){
            esferas.push_back(sphere(p2, *it).size());
        }
        successivespheres.push_back(esferas);
    }
    return successivespheres; // array of arrays with all the sizes of the spheres created at the centres of the vertices inside a ball centered at p1 with radius epsilon
}
