#pragma once
#include <string>

class Named
{
public:
	Named(std::string name);
	void ChangeName(std::string name);
	std::string name;
	int id;
};