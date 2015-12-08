#include "Config.h"
#include "Environment.h"
#include <fstream>
#include <iostream>
#include <string>

Config::Config(std::string path)
{
	this->path = path;
	this->loadFile();
}


Config::~Config()
{
}

void Config::loadFile() {
	std::fstream plik;
	plik.open(this->path, std::ios::in | std::ios::out);
	if (plik.good() == true)
	{
			std::string line, line2;
			while (!plik.eof())
			{
				getline(plik, line);
				line2 = line;
				size_t position = line.find("=");
				if (position <= 0 || position > line.size()) {
					std::cout << "blad skladni configa";
					return;
				}
				line2.erase(0, position + 1);
				line.erase(position, line.size() - position);
				this->data.push_back(std::make_pair(line, line2));
			}

			plik.close();	}
	else {

		std::cout << "blad odczytu configa";
	}
	return;
}

std::string Config::takeConfigString(std::string parameterName) {
	for each (std::pair<std::string, std::string> parameter in this->data)
	{
		if (parameter.first == parameterName) return parameter.second;
	}
	return "";
}
int Config::takeConfigInt(std::string parameterName) {
	try {
		return atoi((char*)this->takeConfigString(parameterName).c_str());
	}
	catch (std::string e) {
		std::cout << "problem z odczytem parametru configu";
	}
}