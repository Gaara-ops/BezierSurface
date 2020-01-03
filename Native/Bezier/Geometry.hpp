#pragma once
#include "Sence.hpp"
#include "Maths.hpp"



namespace Geometry
{
	mat ComputeModelMatrix();
	mat ComputeViewMatrix();
	mat ComputeProjectMatrix();	
	mat ComputeViewportMatrix(float minX,float minY,float maxZ,float minZ,int gWidth,int gHeight);

	mat ComputeMVP();

}

