#include "Named.h"

Named::Named(std::string name)
{
	static int id = 0;
	this->name = name;
	this->id = ++id;
}

void Named::ChangeName(std::string name)
{

	this->name = name;
}
