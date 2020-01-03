#pragma once
#include "Maths.hpp"


struct Mesh
{
	int NumVertex;
	int NumFaces;
	std::vector<vec3f> vertexs;
	std::vector<vec3f> vertexTextures;
	std::vector<vec3f> vertexsNormal;

	// ��Ŵ�1��ʼ���ǵü�1
	std::vector<vec3i> faceVertexIndex;

	// ��Ŵ�1��ʼ���ǵü�1
	std::vector<vec3i> faceTextureIndex;

	// ��Ŵ�1��ʼ���ǵü�1
	std::vector<vec3i> faceNormalIndex;
};



class ObjParser
{  
public:
	static bool ParserMesh(const char*,Mesh&);
};

