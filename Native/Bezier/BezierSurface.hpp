#pragma once
#include "Maths.hpp"
#include "Mesh.hpp"
#include "Sence.hpp"


class BezierSurface
{
public:
	BezierSurface(){ }
	BezierSurface(const char* path,int nTimes);
	void ClearZbuffer();


	vec3f BezierSurfaceTansform(const vec3f& orignVertex);
	float Bernstein(float u,int index);
	static void InputManage();
	~BezierSurface();
	std::vector<float> mZbuffer;
	vec2i mActivePoint;
	Mesh mMesh;
	int Factorial(int n);
	std::vector<std::vector<vec3f>> controlPoints;
	vec2i FindNearestCtrlPoint(const vec2i&,const mat&);
	
private:
	int mNTimes;
	
};

