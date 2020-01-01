#include "Renderer.hpp"
#include "Rasterize.hpp"
#include "BezierCurve.hpp"
#include <iostream>
bool gIsCurve = true;


bool Renderer::Init()
{
	mWidth = 500;
	mHeight = 500 ;
	const char* winTitle = "±´Èû¶ûÇúÏß";
	Platform::MyRegisterClass();
	if(!Platform::InitInstance(mWidth,mHeight,winTitle))
	{
		return false;
	}
	return true;
}

void Renderer::Loop()
{
	if(gIsCurve)	//±´Èû¶ûÇúÏß
	{
		gBezierCurve =  BezierCurve(4);
		Platform::PrintHelpCurve();
		MSG msg;
		while (true)
		{
			if (PeekMessage(&msg,0,0,0,PM_REMOVE))
			{
				if(msg.message == WM_QUIT) 
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			memset(gFbo, 0, mWidth * mHeight * 4);

			gBezierCurve.DrawCurve();

			Platform::UpdateBuffer(mWidth,mHeight);
		}
	}
	else
	{

	}
}




