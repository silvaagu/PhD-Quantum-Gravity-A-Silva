#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "riccicorrelator.hpp"
#include <chrono>

void RicciCorrelator::process() {

	int vmax = 0;
	for (auto v : Universe::vertices) {
		if (v > vmax) vmax = v; //busca índice mas grande de todos los vértices en el Universo
	}
	doneL.resize(vmax + 1, false); // la funcion resize agrega vmax + 1 elementos con valor false al vector. Si ya tenia menos elementos que vmax+1 elementos los agrega al final, y si tenia mas, borra todos los que sobran a partir de vmax mas 1, y pone los que quiere al principio

	int N=epsilons.size();
	int M=distances.size(); //hasta ahora distances solo guarda el numero de puntos que quiero tomar para ver las correlaciones
	std::string tmp = "";
	for (int i=0; i<N;i++){
        for (int i = 0; i < doneL.size(); i++) { // hace falsos todos los vértices de nuevo, para que no queden pintados ninguno al empezar de vuelta
			doneL.at(i) = false;
		}
        auto origen=randomVertex(); // tomo el origen a cualquier tiempo
        auto aux=origen; // empiezo midiendo la correlacion cuando las esferas están totalmente solapadas
//        assert(2*epsilons[i] < ((Universe::sliceSizes[aux->time] -4*epsilons[i])/(2))); // chequeo que no me esté saliendo del ancho maximo que se puede tener
        for (int j=0;j<M;j++){
            tmp += std::to_string(sphere(origen,epsilons[i]).size()); // guardo la esfera con la que inicié
            tmp += " ";
            doneL.at(aux)=true; // pinto con un verdadero el origen de la esfera anterior (marco que ya lo usé como origen)
            for(auto neighbor : Universe::vertexNeighbors[aux]){
                if((neighbor->time == neighbor->time) && (!doneL.at(neighbor)) ){ // esto es para encontrar un vértice con separación tipo espacio =
                    aux=neighbor;
                    tmp += std::to_string(sphere(aux,epsilons[i]).size()); // guardo la esfera con la que inicié
                    tmp += " ";
                    break; // sale del ciclo for una vez que ya encontró el vértice que quería
                }
            }
        }
        tmp += "\n"; // cada nuevo radio es una nueva linea
	}
	tmp.pop_back(); //borro el ultimo salto de linea
    output = tmp; // lo mando a output
}

