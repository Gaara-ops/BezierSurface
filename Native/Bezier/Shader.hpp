#pragma once
#include "Maths.hpp"
#include "Platform.hpp"

struct Shader
{
	virtual ~Shader(){ }
	virtual vec4f vertex(const vec3f& vertex,const vec3f& normal,
		int index,const vec3f& light) = 0;
	virtual Color fragment(vec3f& lerpCorrect) = 0;
};



struct PhongShader : public Shader {
	mat MVP;
	float vary_Intensity[3];


	vec4f vertex(const vec3f& vertex,const vec3f& normal,
		int index,const vec3f& light) override
	{ 
		vary_Intensity[index] = (std::max)(0.0f,normal*light);
		return (MVP * mat(vertex)).ToVec4f();
	}

	Color fragment(vec3f& lerpCorrect) override{
		float normalizer =1/( lerpCorrect.x + lerpCorrect.y + lerpCorrect.z);
		float correct_vary_Intensity = (vary_Intensity[0] * lerpCorrect.x +
										vary_Intensity[1] * lerpCorrect.y +
										vary_Intensity[2] * lerpCorrect.z) * normalizer;
		int inten = 255*correct_vary_Intensity;
		return Color(inten,inten,inten);
	}

};

