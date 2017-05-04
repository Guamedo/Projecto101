#pragma once
#include <string>
#include "Animacion.h"

class Action
{
public:
	Action();
	Action(std::string nombreAnimacion, char* sourceAnimacion);
	~Action();

	std::string nombre;
	Animacion animacion;
};

