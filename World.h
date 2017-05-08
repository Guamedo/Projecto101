#pragma once
class World
{
public:
	World();
	~World();

	bool inrangex(int val);
	bool inrangey(int val);

	char* name = "abc";
	int H = 200, W = 200;
};

