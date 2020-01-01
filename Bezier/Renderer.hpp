#pragma once
#include "Platform.hpp"
#include <iostream>

extern bool gIsCurve;

class Renderer
{
public:
	bool Init();
	void Loop();
	
private:
	
	int mWidth;
	int mHeight;
};

