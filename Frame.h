#pragma once
#include <vector>
#include "Dot.h"

class Frame
{
public:
	Frame();
	~Frame();

	int numDots;
	std::vector<Dot> dots;
};

