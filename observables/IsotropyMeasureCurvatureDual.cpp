#ifndef ISOTROPYMEASURECURVATUREDUAL_CPP_INCLUDED
#define ISOTROPYMEASURECURVATUREDUAL_CPP_INCLUDED

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "IsotropyMeasureCurvatureDual.hpp"
#include <chrono>
#include <cstdlib>

void IsotropyCurvatureDual::process() {
    std::vector<std::vector<std::vector<double>>> epsilonDistanceList;
	std::vector<Triangle::Label> origins;

	int vmax = 0;
	for (auto v : Universe::triangles) {
		if (v > vmax) vmax = v; //busca índice mas grande de todos los triangulos en el Universo (lo hago solo una vez)
	}

    Triangle::Label vaux;
    for (std::vector<int>::iterator it = epsilons.begin(); it != epsilons.end(); it++) {
        do{
            vaux=Universe::trianglesAll.pick();
        }while(!(6 * (*it) + 1 < Universe::sliceSizes[vaux->time])); // si el tamaño del slice donde pone el origen no respeta el tamaño minimo, entonces vuelve a buscar a buscar el origen (evito problemas si la distancia es purament spacelike)
		origins.push_back(vaux); // mete un origen random de una esfera por cada radio en epsilons
	}

	for (int i = 0; i < epsilons.size(); i++) {
		int epsilon = epsilons[i];
		// printf("%d - ", epsilon);

		auto origin = origins[i];

        std::vector<std::vector<double>> averageDistance = DirectionalAverageSphereDistanceDual(origin, epsilon,vmax); // calcula la distancia promedio entre puntos de una esfera con otra que se superpone, ambas con radio epsilon
        epsilonDistanceList.push_back(averageDistance); // la guarda en este array

		// printf("%f\n", averageDistance);
    }

    std::string tmp = "";


    for (auto radio : epsilonDistanceList){ // para cada radio (uno abajo del otro)
        for (auto tipo : radio) { // va a escribir de arriba a abajo, los distintos tipos (pone uno abajo del otro): primero todos los tipos, luego los spacelike, luego los timelike
                for (double dst : tipo){ // para cada tipo, pone uno al lado del otro los valores de distancias promedio en cada una de las direcciones
                    tmp += std::to_string(dst); // para cada medicion, pone las distancias promedio una al lado de la otra en el archivo
                    tmp += " ";
                }
            tmp += "\n";
        }
    }
    output = tmp;
}

std::vector<std::vector<double>> IsotropyCurvatureDual::DirectionalAverageSphereDistanceDual(Triangle::Label p1, int epsilon,int vmaxx) {
    int T=Universe::nSlices;

    std::vector<double> directionaldistances;
    std::vector<double> SpaceLikedirectionaldistances;
    std::vector<double> TimeLikedirectionaldistances;
    std::vector<std::vector<double>> directionaldistancesSLBounds;


    for(auto p2 : sphereDual(p1, epsilon)){
//        std::vector<bool> doneL; //array de verdaderos o falsos
//        std::vector<bool> triangleL;
        doneL.resize(0); //I delete the memory of this vector in case (for some reason) it is storing more information that it should
        triangleL.resize(0);
        doneL.resize(vmaxx + 1, false); //array de verdaderos o falsos
        triangleL.resize(vmaxx + 1, false);
        auto s1 = sphereDual(p1, epsilon); //crea esfera centrada en p1
        int t1 = p1->dualtime; // me fijo el tiempo del centro de la esfera
        auto s2 = sphereDual(p2, epsilon); // crea una esfera al rededor de ese punto
        int t2 = p2->dualtime; // me fijo el tiempo del centro de la siguiente esfera
        if (s2.size() < s1.size()) { // para continuar, me aseguro que la esfera mas chica sea la primera, si no es así, las intercambio para tener como s1 a la mas pequeña
            auto stmp = s1;
            s1 = s2;
            s2 = stmp;
        }

        //int tmin = 0, tmax = 0;
        //// these bounds can be improved
        //tmax = (t1 + 5 * std::max(epsilon, 2) / 2 + 1) % Universe::nSlices;
        //tmin = (t1 - (5 * std::max(epsilon, 2) / 2 + 1) + Universe::nSlices) % Universe::nSlices;

        assert(3 * epsilon + 1 < Universe::nSlices); // me fijo que el radio que uso no supere la distancia máxima permitida en una topología toroidal

        std::vector<int> distanceList; // armo una lista de distancias entre esferas

        ///using std::chrono::high_resolution_clock;
        ///using std::chrono::duration_cast;
        ///using std::chrono::duration;
        ///using std::chrono::milliseconds;

        ///auto t1 = high_resolution_clock::now();
        for (auto b : s1) { // para todos los vertices dentro de la esfera 1
            for (int i = 0; i < doneL.size(); i++) { // hace falsos todos los vértices de la esfera 1
                doneL.at(i) = false;
                triangleL.at(i) = false;
            }
            for (auto v : s2) { //hace verdaderos todos los vértices de la esfera 2
                triangleL.at(v) = true;
            }

            int countdown = s2.size(); //numero que va haciendo una cuenta regresiva sobre todos los vértices de la esfera que hay que mirar

            std::vector<Triangle::Label> thisDepth;
            std::vector<Triangle::Label> nextDepth;

            doneL.at(b) = true; //transforma en verdadero uno de los vértices de la esfera 1
            thisDepth.push_back(b); //lo mete en este vector

            for (int currentDepth = 0; currentDepth < 3 * epsilon + 1; currentDepth++) { // mira todo lo que está dentro de un radio menor o igual a 3 epsilon
                for (auto v : thisDepth) { //se fija para todos los vértices que hay en la esfera 1
                    if (triangleL.at(v)) { // se fija si el vértice de la esfera está en la lista de fértices de la esfera 2
                        distanceList.push_back(0); // le pone distancia cero
                        triangleL.at(v) = false; // lo toma como vertice ya mirado
                        countdown--; // reduce la cantidad de vertices a mirar en 1, porque este ya lo tuvo en cuenta
                    }

                    for (auto neighbor : Universe::triangleNeighbors[v]) { // ahora mira todos los vertices vecinos
                        //if (neighbor->time == tmax || neighbor->time == tmin) continue;
                        if (!doneL.at(neighbor)) { // se fija si ya lo miró
                            nextDepth.push_back(neighbor); //lo agrega como un punto base para la siguiente iteracion
                            doneL.at(neighbor) = true; // como ya lo miró, le pone verdadero

                            if (triangleL.at(neighbor)) { // si uno de los vecinos del vertice pertenece a la otra esfera, entonces entra
                                distanceList.push_back(currentDepth + 1); //mide la cantidad de pasos que le costó llegar a ese punto, basicamente la cantidad de links
                                triangleL.at(neighbor) = false;
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
            thisDepth.clear();
            nextDepth.clear();
        }
        //auto t2 = high_resolution_clock::now();

        //auto ms_int = duration_cast<milliseconds>(t2 - t1);
        //printf("eps: %d, t: %d\n", epsilon, ms_int);

        int distanceSum = std::accumulate(distanceList.begin(), distanceList.end(), 0);
        double averageDistance = static_cast<double>(distanceSum)/static_cast<double>(epsilon*distanceList.size()); //distanceList.size() es la cantidad de vertices de una esfera*la cantidad de vertices de la otra. la cantidad de vértices en el medio, se obtiene simlemente multiplicando por el radio(numero de pasos hasta llegar a cada vertice. Asi se obtiene N1(epsilon)*N2(epsilon), de ambas esferas

        directionaldistances.push_back(averageDistance);
        SpaceLikedirectionaldistances.push_back(averageDistance); //les pongo lo mismo solo para poder analizar mas facil, porque no puedo separar todavía en SL y TL
        TimeLikedirectionaldistances.push_back(averageDistance); //les pongo lo mismo solo para poder analizar mas facil, porque no puedo separar todavía en SL y TL
//        if(t1==t2){
//            SpaceLikedirectionaldistances.push_back(averageDistance);
//        }
//        if((abs(t1-t2)==epsilon) || (abs(t2-t1)==T-epsilon) ){
//            TimeLikedirectionaldistances.push_back(averageDistance);
//        }
    s1.clear();
    s2.clear();
    }
    directionaldistancesSLBounds.push_back(directionaldistances);
    directionaldistancesSLBounds.push_back(SpaceLikedirectionaldistances);
    directionaldistancesSLBounds.push_back(TimeLikedirectionaldistances);
    return directionaldistancesSLBounds; // el primer valor es el array de distancias con todos los vecinos, el segundo solo con los spacelike, y el tercero con los timelike
}
#endif // ISOTROPYMEASURECURVATUREDUAL_CPP_INCLUDED
