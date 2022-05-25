// Copyright 2020 Joren Brunekreef and Andrzej G�rlich
#include <string>
#include "volume_profile.hpp"

void VolumeProfile::process() {
	std::string tmp = ""; //crea string vac�o
	for (auto l : Universe::sliceSizes) { //hace un for que corre sobre todos los tama�aos de cada slice del universo
		tmp += std::to_string(l); // va agregando uno al lado del otro separado por espacio,
		tmp += " "; // separado por un espacio, los tama�os de cada slice
	}
	tmp.pop_back(); // remueve el �ltimo elemento (???) no se por qu�

	output = tmp; // lo pone en output (basucamente el archivo de salida que le dimos a universo)
}
