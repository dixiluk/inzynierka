#pragma once
#include <list>
class Config
{
private:
	std::list<std::pair<std::string, std::string>> data;
	std::string path;

public: 

	Config(std::string path);
	~Config();
	void loadFile();
	std::string takeConfigString(std::string parameterName);
	int takeConfigInt(std::string parameterName);
};
