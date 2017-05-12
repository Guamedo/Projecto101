#pragma once
#include <string>

class World
{
public:
	World();
	~World();

	bool inrangex(int val);
	bool inrangey(int val);

	std::string name = "abc";
	int H = 200, W = 200;
};

