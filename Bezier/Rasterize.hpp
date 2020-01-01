#pragma once
#include "Maths.hpp"
#include "Platform.hpp"

class Rasterize
{
public:
	static void DrawLine(const vec2i&,const vec2i&,const Color&);
	static void DrawBox(const vec2i&,const int,const Color&);
};

