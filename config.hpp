#ifndef CONFIG_HPP_INCLUDED
#define CONFIG_HPP_INCLUDED

// Copyright 2020 Joren Brunekreef and Andrzej Görlich
#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <cassert>

class ConfigReader {
public:
	void read(std::string fname) {
		std::ifstream infile(fname); // abre el archivo con el nombre fname
		assert(infile.is_open()); //chequea si está abierto
		std::string key, value; // palabra y numeros asociados a cada palabra

		while (infile >> key >> value) { // extracts key and value from the file and stores them in key and value
			dict[key] = value; // saves the dictionary associated with each key and value
		}

		assert(dict.find("lambda") != dict.end()); //checks everything is okey, and that the file is not reading the end of file
		assert(dict.find("targetVolume") != dict.end());
		assert(dict.find("slices") != dict.end());
		assert(dict.find("sphere") != dict.end());
		assert(dict.find("seed") != dict.end());
		assert(dict.find("fileID") != dict.end());
		assert(dict.find("measurements") != dict.end());
		assert(dict.find("geom_in") != dict.end());
		assert(dict.find("geom_out") != dict.end());
	}

	int getInt(std::string key) {
		return std::stoi(dict[key]); // returns the value associated with the key given
	}

	double getDouble(std::string key) {
		return std::stod(dict[key]);
	}

	std::string getString(std::string key) {
		return dict[key];
	}

private:
	std::unordered_map<std::string, std::string> dict; //basically, this thing searches the second argument inside the first one
};



#endif // CONFIG_HPP_INCLUDED
