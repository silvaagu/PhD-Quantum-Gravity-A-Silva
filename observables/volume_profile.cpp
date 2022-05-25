// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#include <string>
#include "volume_profile.hpp"

void VolumeProfile::process() {
	std::string tmp = ""; //crea string vacío
	for (auto l : Universe::sliceSizes) { //hace un for que corre sobre todos los tamañaos de cada slice del universo
		tmp += std::to_string(l); // va agregando uno al lado del otro separado por espacio,
		tmp += " "; // separado por un espacio, los tamaños de cada slice
	}
	tmp.pop_back(); // remueve el último elemento (???) no se por què

	output = tmp; // lo pone en output (basucamente el archivo de salida que le dimos a universo)
}
