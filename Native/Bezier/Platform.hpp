#pragma once
#include <Windows.h>
#include "Maths.hpp"
#include "BezierCurve.hpp"
#include "BezierSurface.hpp"

struct SenceParameters
{
	//模型属性
	vec3f gMeshMove;	 // 模型在世界空间下偏移
	vec3f gMeshRotate;	 // 模型旋转值
	vec3f gMeshScale;	 // 模型缩放值

						 //灯光属性
	vec3f gLight;		//灯光位置

						// 相机属性
	vec3f gEye;			//相机在世界空间下的位置
	vec3f gAt;			//相机视点
	vec3f gUp;			//相机向上方向向量
	float gAspect;		//宽高比
	float gFovy;		//视野
	float gFarZ;		//近平面
	float gNearZ;		//远平面
};
class BezierSurface;
extern BezierCurve gBezierCurve;
extern BezierSurface gBezierSurface;
extern HWND ghMainWnd;	
extern HDC ghdcMainWnd;						
extern HBITMAP gdibMap;						
extern HBITMAP goldMap;		
extern int gInputKeys[512];
extern unsigned char* gFbo;
extern int gWidth;
extern int gHeight;
extern int gMouseCtrlPoint;
extern vec2i gSurfaceCtrlPoint;



namespace Platform
{
	void MyRegisterClass();
	BOOL InitInstance(int,int,const char*);
	void SetPixel(const int&,const int&,const Color&);
	void UpdateBuffer(int&,int&);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    void PrintHelpCurve();
	void PrintHelpSurface();
};

