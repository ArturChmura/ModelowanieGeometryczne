#include "Named.h"

Named::Named(std::string name)
{
	static int id = 0;
	this->id = ++id;
	this->name = name;
}

void Named::ChangeName(std::string name)
{

	this->name = name;
}
