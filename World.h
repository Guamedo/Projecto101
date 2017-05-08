#pragma once
#include <string>

class World
{
public:
	World();
	~World();

	bool inrangex(int val);
	bool inrangey(int val);

	std::string name = "Pixel Man 3000 Ultra Pro";
	int H = 300, W = 300;
};

