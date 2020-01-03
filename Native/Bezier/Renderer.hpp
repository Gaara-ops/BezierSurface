#pragma once
#include "Platform.hpp"
#include "Sence.hpp"
#include <iostream>



extern bool gIsCurve;

class Renderer
{
public:
	Renderer(int width,int height,bool mode){
		mWidth = width;
		mHeight = height ;
		gIsCurve = mode;
	}
	bool Init();
	void Loop();
	
private:
	int mWidth;
	int mHeight;
};

