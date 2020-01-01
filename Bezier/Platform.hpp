#pragma once
#include <Windows.h>
#include "Maths.hpp"
#include "BezierCurve.hpp"

extern HWND ghMainWnd;	
extern HDC ghdcMainWnd;						
extern HBITMAP gdibMap;						
extern HBITMAP goldMap;		
extern int gInputKeys[512];
extern unsigned char* gFbo;
extern int gWidth;
extern int gHeight;
extern BezierCurve gBezierCurve;
extern int gMouseCtrlPoint;

namespace Platform
{
	void MyRegisterClass();
	BOOL InitInstance(int,int,const char*);
	void SetPixel(const int&,const int&,const Color&);
	void UpdateBuffer(int&,int&);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    void PrintHelpCurve();
};

