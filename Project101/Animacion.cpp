#include "stdafx.h"
#include "Animacion.h"


Animacion::Animacion()
{
}

Animacion::Animacion(int cantidad, std::vector<Frame> frames) {
	numFrames = cantidad;
	frames = frames;
}

Animacion::Animacion(std::string archivo) {
	std::fstream file;
	int x, y;
	file.open(archivo);
	int aux = 0;
	file >> numFrames;
	frames = std::vector<Frame>(numFrames, Frame());
	for (int i = 0; i < numFrames; i++) {
		file >> frames.at(i).numDots;
		frames[i].dots = std::vector<Dot>(frames.at(i).numDots, Dot());
		for (int j = 0; j < frames.at(i).numDots; j++) {
			file >> frames.at(i).dots.at(j).x;
			file >> frames.at(i).dots.at(j).y;
		}
	}

}

Animacion::~Animacion()
{
}
