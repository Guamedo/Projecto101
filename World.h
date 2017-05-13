#pragma once
#include <string>

class World
{
public:
	World();
	World(std::string name, int h, int w, float g);
	~World();

	bool inrangex(double val);
	bool inrangey(double val);

	std::string name = "El mundo de J";
	int H = 200, W = 200;
	float gravity = 9.8;
};

