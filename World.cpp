#include "World.h"


World::World()
{
}


World::~World()
{
}

bool World::inrangex(int val) {
	if (10 < val && val < W - 10) {
		return true;
	}
	else {
		return false;
	}
}
bool World::inrangey(int val) {
	if (10 < val && val < H - 10) {
		return true;
	}
	else {
		return false;
	}
}