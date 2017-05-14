#include <string>
#include "Box.h"
#include <vector>

class World
{
public:
	World();
	World(std::string name, int h, int w, float g);
	~World();

	bool inrangex(double val);
	bool inrangey(double val);
	std::vector<Box> *getPlatforms();
	void loadLevel(std::string name);

	std::string name = "El mundo de J";
	int H = 200, W = 200;
	float gravity = 9.8;
	std::vector<Box> platforms;

};

