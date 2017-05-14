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
    file.open(level);
    double x, a, b, c, d;
    file >> x;
    for (int i = 0; i < x; i++) {
        file >> a >> b >> c >> d;
        platforms.push_back(Box({a, b}, {c, d}));
    }
    file.close();
}