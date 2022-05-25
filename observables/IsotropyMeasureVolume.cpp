#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "IsotropyMeasureVolume.hpp"
#include <chrono>

void IsotropyVolume::process() {
    std::vector<std::vector<double>> epsilonVolumesList;
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

        std::vector<double> volumes = DirectionalVolume(origin, epsilon); // calcula la distancia promedio entre puntos de una esfera con otra que se superpone, ambas con radio epsilon
        epsilonVolumesList.push_back(volumes); // la guarda en este array

		// printf("%f\n", averageDistance);
    }

    std::string tmp = "";
    for (auto radiusarray : epsilonVolumesList) { // va a escribir de arriba a abajo, en radio creciente
            for (double vol : radiusarray){ // para cada radio, pone uno al lado del otro los valores de areas de cada esfera en cada una de las direcciones
                tmp += std::to_string(vol); // para cada medicion, pone las areas de cada esfera una al lado de la otra en el archivo
                tmp += " ";
            }
        tmp += "\n";
    }
    output = tmp;
}

std::vector<double> IsotropyVolume::DirectionalVolume(Vertex::Label p1, int epsilon) {
    auto s1 = sphere(p1, epsilon); //crea esfera centrada en p1
	int t1 = p1->time; // me fijo el tiempo del centro de la esfera

    std::vector<double> volumeList; // armo una lista de areas de esferas
    for(auto p2 : s1){
            auto s2 = sphere(p2, epsilon); // crea una esfera al rededor de ese punto
        assert(6 * epsilon + 1 < Universe::nSlices); // me fijo que el radio que uso no supere la distancia máxima permitida en una topología toroidal
        volumeList.push_back(s2.size()); //mete el volumen de esa esfera
        }
    return volumeList;
}
