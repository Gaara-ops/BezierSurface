#include "BezierSurface.hpp"
#include "Platform.hpp"
#include "BezierCurve.hpp"
#include <iostream>

BezierSurface::BezierSurface(const char* path,int nTimes)
{
	mActivePoint = vec2i(-1,-1);
	ObjParser::ParserMesh(path,mMesh);
	 mZbuffer = std::vector<float>(gWidth*gHeight);
	/* mNTimes = nTimes;
	 controlPoints = std::vector<std::vector<vec3f>>(nTimes+1,vvec3f(nTimes+1,vec3f(0.0,0.0,0.0)));*/

	 //n = 4测试
	 mNTimes = 3;
	 std::vector<vec3f> a;
	 a.push_back(vec3f(-10,10,0));
	 a.push_back(vec3f(-0,7,0));
	 a.push_back(vec3f(15,3,0));
	 a.push_back(vec3f(30,8,0));
	 controlPoints.push_back(a);

	 std::vector<vec3f> b;
	 b.push_back(vec3f(-10,0,10));
	 b.push_back(vec3f(-5,15,10));
	 b.push_back(vec3f(20,10,10));
	 b.push_back(vec3f(30,5,10));
	 controlPoints.push_back(b);

	 std::vector<vec3f> c;
	 c.push_back(vec3f(-10,5,20));
	 c.push_back(vec3f(-5,-10,20));
	 c.push_back(vec3f(10,10,20));
	 c.push_back(vec3f(30,0,20));
	 controlPoints.push_back(c);

	 std::vector<vec3f> d;
	 d.push_back(vec3f(-10,4,30));
	 d.push_back(vec3f(-5,8,30));
	 d.push_back(vec3f(20,6,30));
	 d.push_back(vec3f(30,4,30));
	 controlPoints.push_back(d);
}

void BezierSurface::ClearZbuffer()
{
	for (int i=gWidth*gHeight; i--; mZbuffer[i] = 10000.0f);
}

vec3f BezierSurface::BezierSurfaceTansform(const vec3f& orignVertex)
{
	//转化为区间 0-1
	float u = orignVertex.x + 0.5f;
	float w = orignVertex.z + 0.5f;

	
	vec3f result = vec3f(0.0,0.0,0.0);
	for(int i = 0; i<mNTimes +1 ; i++)
	{
		for(int j = 0; j<mNTimes +1 ; j++)
		{
			float tmp = Bernstein(u,i) * Bernstein(w,j);
			result.x += tmp * controlPoints[i][j].x;
			result.y += tmp * controlPoints[i][j].y;
			result.z += tmp * controlPoints[i][j].z;
		}
		
	}
	return result;
}

float BezierSurface::Bernstein(float u,int index)
{
	float factor_0 = Factorial(mNTimes) / (Factorial(mNTimes - index) * Factorial(index));
	float factor_1 = pow((1 - u),(mNTimes - index));
	float factor_3 = pow(u,index);
	return  factor_0 * factor_1 * factor_3;
}

void BezierSurface::InputManage()
{
	if(gInputKeys[VK_UP])
	{
		int i = gSurfaceCtrlPoint.x;
		int j = gSurfaceCtrlPoint.y;
		if(i>=0) //选中模式
		{
			gBezierSurface.controlPoints[i][j].z += 0.1f; 
		}
		else
		{
			gMeshScale = gMeshScale * 1.01f;
			Platform::PrintHelpSurface();
		}
		
	}
		
	if(gInputKeys[VK_DOWN])
	{
		int i = gSurfaceCtrlPoint.x;
		int j = gSurfaceCtrlPoint.y;
		if(i>=0) //选中模式
		{
			gBezierSurface.controlPoints[i][j].z -= 0.1f; 
		}else{
			gMeshScale = gMeshScale * 0.99f;
			Platform::PrintHelpSurface();
		}
		
	}
		
	if(gInputKeys[VK_LEFT])
	{
		gMeshRotate.y += 2.0f;
		Platform::PrintHelpSurface();
	}
		
	if(gInputKeys[VK_RIGHT])
	{
		gMeshRotate.y -= 2.0f;
		Platform::PrintHelpSurface();
	}
		

	if(gInputKeys[0x57])//W
	{
		int i = gSurfaceCtrlPoint.x;
		int j = gSurfaceCtrlPoint.y;
		if(i>=0) //选中模式
		{
			gBezierSurface.controlPoints[i][j].y += 0.1f; 
		}
		else
		{
			gMeshMove.y += 0.04f;
			Platform::PrintHelpSurface();
		}
	}
		
	if(gInputKeys[0x53])//S
	{
		int i = gSurfaceCtrlPoint.x;
		int j = gSurfaceCtrlPoint.y;
		if(i>=0) //选中模式
		{
			gBezierSurface.controlPoints[i][j].y -= 0.1f; 
		}
		else
		{
			gMeshMove.y -= 0.04f;
			Platform::PrintHelpSurface();
		}
		
	}
		
	if(gInputKeys[0x41])//A
	{
		int i = gSurfaceCtrlPoint.x;
		int j = gSurfaceCtrlPoint.y;
		if(i>=0) //选中模式
		{
			gBezierSurface.controlPoints[i][j].x += 0.1f; 
		}else{
			gMeshMove.x += 0.04f;
			Platform::PrintHelpSurface();
		}
		
	}
		
	if(gInputKeys[0x44])//D
	{
		int i = gSurfaceCtrlPoint.x;
		int j = gSurfaceCtrlPoint.y;
		if(i>=0) //选中模式
		{
			gBezierSurface.controlPoints[i][j].x -= 0.1f; 
		}else{
			gMeshMove.x -= 0.04f;
			Platform::PrintHelpSurface();
		}
	}
		
		
	if(gInputKeys[0x52])//R
	{
		SenceReset();
		Platform::PrintHelpSurface();
	}


		

	//灯光控制
	if (gInputKeys[0x49])//I
	{
		gLight.y += 0.04f;
		gLight.normalize();
		Platform::PrintHelpSurface();
	}
	if(gInputKeys[0x4B])//K
	{
		gLight.y -= 0.04f;
		gLight.normalize();
		Platform::PrintHelpSurface();
	}
	if(gInputKeys[0x4A])//J
	{
		gLight.x += 0.04f;
		gLight.normalize();
		Platform::PrintHelpSurface();
	}
	if(gInputKeys[0x4C])//L
	{
		gLight.x -= 0.04f;
		gLight.normalize();
		Platform::PrintHelpSurface();
	}
	if(gInputKeys[0x55])//U
	{
		gLight.z += 0.04f;
		gLight.normalize();
		Platform::PrintHelpSurface();
	}
	if(gInputKeys[0x4F])//O
	{
		gLight.z -= 0.04f;
		gLight.normalize();
		Platform::PrintHelpSurface();
	}


	
	
}



BezierSurface::~BezierSurface()
{
	//delete[] mZbuffer;
}

int BezierSurface::Factorial(int n)
{
	if(n == 0) return 1;
	int factorial = 1;
	for(int i = 1; i <=n; ++i)
	{
		factorial *= i;
	}
	return factorial;
}

vec2i BezierSurface::FindNearestCtrlPoint(const vec2i & mouse_pos,const mat& MVP)
{
	mat tmp = MVP;
	vec2i index = vec2i(-1,-1);
	float Min_len = 10000.0f;
	for(int i = 0; i<4; i++)
	{
		

		for(int j = 0; j<4; j++)
		{
			vec3f t_point = controlPoints[i][j];
			//MVP变换
			vec4f result =  (tmp * mat(t_point)).ToVec4f();
			//透视除法
			result = result * (1/result.w);
			//屏幕坐标转换
			int screen_coord_x = (int)( (result.x + 1.0f)* gWidth / 2.f);
			int screen_coord_y = (int)( (result.y + 1.0f)* gHeight / 2.f);

			vec2i tt = (mouse_pos - vec2i(screen_coord_x,screen_coord_y));
			float len = (tt.x*tt.x + tt.y*tt.y);
			if(len<Min_len )
			{
				Min_len = len;
				index.x = i;
				index.y = j;
			}
		}
	}

	if(Min_len > 50) return vec2i(-1,-1);
	//debug
	//std::cout<<index.x<<","<<index.y<<std::endl;
	return index;
}
