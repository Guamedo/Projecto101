#include <fstream>
#include "World.h"


World::World()
{
}

World::World(std::string name, int h, int w, float g) {
	this->name = name;
	this->H = h;
	this->W = w;
	this->gravity = g;
}

World::~World()
{
}

bool World::inrangex(double val) {
	if (10 < val && val < W - 10) {
		return true;
	}
	else {
		return false;
	}
}
bool World::inrangey(double val) {
	if (10 < val && val < H - 10) {
		return true;
	}
	else {
		return false;
	}
}

std::vector<Box> *World::getPlatforms(){
    return &platforms;
}

void World::loadLevel(std::string level) {
    std::fstream file;
	std::string line;
	file.open(level);
	float a, b, c, d;
	while(getline(file,line)){
		if(!line.empty() && line[0] != '#' && isdigit(line[0])){
			sscanf(line.c_str(),"%f %f %f %f", &a, &b, &c, &d);
			platforms.push_back(Box({a, b}, {c, d}));
		}
	}
	file.close();
}