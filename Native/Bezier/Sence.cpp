#include "Sence.hpp"
#include <sstream>

vec3f gMeshMove	= {0.0f,0.8f,0.0f};
vec3f gMeshRotate = {0.0f,0,180.f};
vec3f gMeshScale	= {0.1f,0.1f,0.1f};
bool gBanOutput = false;
vec3f gLight		= {0.0f,1.0f,0.0f};
mat gMVP;
float gAspect		= 1.0f;
vec3f gEye		= {0.0f,0.0f,-3.0f};
vec3f gAt			= {0.0f,0.0f,0.0f};	
vec3f gUp			= {0.0f,1.0f,0.0f};	

float gFovy		= 60;
float gFarZ		= 0.1f;
float gNearZ		= 100.0f;

std::string SenceParameterOutPut()
{	
	std::stringstream ss;
	ss<<" Light Position:  "<<"["<<gLight.x<<","<<gLight.y<<","<<gLight.z<<"]"<<std::endl;
	ss<<" Mesh  Move:      "<<"["<<gMeshMove.x<<","<<gMeshMove.y<<","<<gMeshMove.z<<"]"<<std::endl;
	ss<<" Mesh  Rotate:    "<<"["<<gMeshRotate.x<<","<<gMeshRotate.y<<","<<gMeshRotate.z<<"]"<<std::endl;
	ss<<" Mesh  Scale:     "<<"["<<gMeshScale.x<<","<<gMeshScale.y<<","<<gMeshScale.z<<"]"<<std::endl;
	return ss.str();
}

void SenceReset()
{
	gMeshMove	= {0.0f,0.0f,0.0f};
	gMeshRotate = {0.0f,0,180.f};
	gMeshScale	= {0.05f,0.05f,0.05f};

	 gLight		= {0.44f,-0.7f,0.55f};

	 gAspect		= 1.0f;
	 gEye		= {0.0f,0.0f,-3.0f};
	 gAt			= {0.0f,0.0f,0.0f};	
	 gUp			= {0.0f,1.0f,0.0f};	

	 gFovy		= 60;
	 gFarZ		= 0.1f;
	 gNearZ		= 100.0f;
}
