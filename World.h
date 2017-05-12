#pragma once
#include <string>

class World
{
public:
	World();
	World(std::string name, int h, int w, float g);
	~World();

	bool inrangex(int val);
	bool inrangey(int val);

	std::string name = "El mundo de J";
	int H = 200, W = 200;
	float gravity = 9.8;
};

