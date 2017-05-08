#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Frame.h"
#include "Dot.h"

class Animacion
{
public:
	Animacion();
	Animacion(int cantidad, std::vector<Frame> frames);
	Animacion(std::string archivo);
	~Animacion();

	int numFrames;
	std::vector<Frame> frames;
};

