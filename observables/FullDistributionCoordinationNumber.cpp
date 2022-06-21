#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "FullDistributionCoordinationNumber.hpp"
#include <chrono>
#include <iostream>
using namespace std;

void FullDistributionCoordinationNumber::process() {

    std::string filename = data_dir + name + "-"+"rici-"+to_string(VolumenUniverso)+"-"+to_string(TimeSlices)+"-"+to_string(123)+extension;
    std::ofstream file;
    file.open(filename, std::ios::app); // abro el archivo aca mismo, asi no tengo que guardar arrays largos en la RAM

	assert(file.is_open());

    int vmax = 0;
	for (auto v : Universe::vertices) {
		if (v > vmax) vmax = v; //busca índice mas grande de todos los vértices en el Universo (lo hago solo una vez)
	}

    int radmax = epsilons[epsilons.size()-1]; // extrae el ultimo elemento del vector de radios, que vendria a ser el maximo

    file << "";
    for (auto v : Universe::vertices ){
    //    std::vector<std::vector<double>> epsilonCNumberList;
        if(((4 * (radmax) + 1 < Universe::sliceSizes[v->time]))){
            for (int i = 0; i < epsilons.size(); i++) {
                int epsilon = epsilons[i];
    //                    epsilonCNumberList.push_back(CoordinationNumberBall(v,epsilon,vmax));
    //                }
    //        }
    //
    //
    //
    //     // para cada vertice en el universo, pone uno abajo del otro las listas de radio creciente, con el coordination number de cada una de sus bolas
    //        for (auto radiusarray : epsilonCNumberList) { // va a escribir de arriba a abajo, en radio creciente
                std::vector<double> radiusarray=CoordinationNumberBall(v,epsilon,vmax);
                for (double cn : radiusarray){ // para cada radio, pone uno al lado del otro los valores de los numeros de coordinacion de cada punto en la bola
    //                tmp += std::to_string(cn);
    //                tmp += " ";
                    file << std::to_string(cn) + " ";
                }
    //            tmp += "\n";
                file << "\n";
            }
        }
    }
//    tmp += "\n";
//    tmp += "F";
//    tmp += "\n";
    file << "\n" << "F" << "\n";
    file.close();
    output="The file is being written inside the process function";
//    output = "\n"; // voy a escribir solamente lo ultimo que quiero en el output file, asi c++ no tiene que guardar esa memoria
//    output = tmp;
}

std::vector<double> FullDistributionCoordinationNumber::CoordinationNumberBall(Vertex::Label origin, int epsilon, int vmaxx) {
 std::vector<double> cnumbers;
    std::vector<Vertex::Label> bola=ball(origin, epsilon);
    for(auto p2 : bola){
        cnumbers.push_back(Universe::vertexNeighbors[p2].size());
    }
    return cnumbers; // array with all the coordination numbers of the vertices inside a ball centered at p1 with radius epsilon
}

//
//	for (auto v : Universe::vertices ){
//        if(((4 * (radmax) + 1 < Universe::sliceSizes[v->time]))){
//            eachvertexepsilonCNumberList.push_back(CoordinationNumberBall(v,vmax));
//        }
//	}
//
//    std::string tmp = "";
//    for (auto eachvertexepsCNumberList : eachvertexepsilonCNumberList){ // para cada vertice en el universo, pone uno abajo del otro las listas de radio creciente, con el coordination number de cada una de sus bolas
//        for (auto radiusarray : eachvertexepsCNumberList) { // va a escribir de arriba a abajo, en radio creciente
//                for (double cn : radiusarray){ // para cada radio, pone uno al lado del otro los valores de los numeros de coordinacion de cada punto en la bola
//                    tmp += std::to_string(cn);
//                    tmp += " ";
//                }
//            tmp += "\n";
//        }
//    }
//    output = tmp;
//}
//
//std::vector<std::vector<double>> FullDistributionCoordinationNumber::CoordinationNumberBall(Vertex::Label origin, int vmaxx) {
//    cout << "hola ";
//    std::vector<Vertex::Label> thisDepth;
//    std::vector<Vertex::Label> nextDepth;
//	std::vector<bool> doneL;
//	doneL.resize(vmaxx + 1, false);
//
//	doneL.at(origin) = true;
//    thisDepth.push_back(origin);
//
//    std::vector<Vertex::Label> vertexList;
//    std::vector<std::vector<Vertex::Label>> bolas;
//    cout << "hola 1 ";
//    for (int i = 0; i < epsilons.size(); i++) {
//        int radius = epsilons[i];
//        for (int currentDepth = 0; currentDepth < radius; currentDepth++) { // cuenta la cantidad de pasos que hace hasta llegar al borde, se queda en uno antes, porque obtiene la esfera mirando los vecinos de los que están en el interior del borde
//            if(currentDepth==0 && i==0){
//                    vertexList.push_back(origin);
//            }
//            for (auto v : thisDepth) {
//                for (auto neighbor : Universe::vertexNeighbors[v]) {
//                    if (!doneL.at(neighbor)) { // si todavi no lo miro, entra
//                       nextDepth.push_back(neighbor); //el proximo punto de apoyo para ver la esfera se transforma en este vertice
//                       doneL.at(neighbor) = true; // como ya lo miro, lo anota como verdadero
//                       if (currentDepth < radius - 1) vertexList.push_back(neighbor); //si la distancia actual es menor al radio, lo guarda como un vértice de la bola
//                       if (currentDepth = radius - 1) {
//                        vertexList.push_back(neighbor);
//                        bolas.push_back(vertexList); // si la distancia altual es igual a la bola, entonces guarda una copia de todos los vertices que estan adentro. hace lo mismo para cada radio
//                       }
//                   }
//                }
//            }
//            thisDepth = nextDepth;
//            nextDepth.clear();
//        }
//    }
//    cout << "hola 2 ";
//    std::vector<std::vector<double>> eachradiuscnumbers;
//    for (auto b: bolas ){ // para cada una de las bolas (cada radio), va a mirar los coordination numbers
//        std::vector<double> cnumbers;
//        for(auto v : b){ // en cada bola mira los coord numbers
//        cnumbers.push_back(Universe::vertexNeighbors[v].size());
//        }
//        eachradiuscnumbers.push_back(cnumbers); // guarda los coord numbers de cada radio, en orden creciente creo
//    }
//    cout << "hola 3 ";
//    cout << eachradiuscnumbers.size() << " ";
//    return eachradiuscnumbers; // array with all the coordination numbers of the vertices inside a ball centered at p1, each element is a possible radius inside epsilons
