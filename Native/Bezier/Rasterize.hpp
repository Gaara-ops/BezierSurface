#pragma once
#include "Maths.hpp"
#include "Platform.hpp"
#include "Shader.hpp"

class Rasterize
{
public:
	static void DrawLine(const vec2i&,const vec2i&,const Color&);
	static void DrawBox(const vec2i&,const int,const Color&);
	static void RasterizeTriangle(vec4f SV_vertexs[3],Shader& shader);
	static vec3f Barycentric(vec2f A,vec2f B,vec2f C,vec2f P);
};

