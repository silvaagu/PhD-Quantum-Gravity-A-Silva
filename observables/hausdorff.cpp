// Copyright 2020 Joren Brunekreef and Andrzej G�rlich
#include <vector>
#include <string>
#include "hausdorff.hpp"

void Hausdorff::process() {
    std::string tmp = "";
	max_epsilon = Universe::nSlices/2; // el maximo radio que puede tener una esfera es la mitad del tama�o del universo, para que la eesfera que tome no sea mas grande que el uiverso en si.
    for (int i = 1; i < max_epsilon; i++) { // tomo esferas con radio creciente, sin llegar a ocupar todo el universo
		auto v = randomVertex(); // tomo un v�rtice random

        std::vector<Vertex::Label> s1 = sphere(v, i); // creo una esfera centrada en un v�rtice, con radio i
        tmp += std::to_string(s1.size()); //imprimo el tama�o de la esfera (cantidad de vertices en el borde)
        tmp += " "; // los voy imprimiendo uno al lado del otro
    }
	tmp.pop_back(); //saco el ultimo espacio del string

    output = tmp;
}
