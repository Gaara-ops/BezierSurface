#include "BezierCurve.hpp"
#include "Platform.hpp"
#include "Rasterize.hpp"
#include <math.h>
#include <sstream>


BezierCurve::BezierCurve(int _n)
{
	nTimes = _n;
	mlineColor = Color(255,255,255);
	mBoxColor = Color(255,255,0);
	mCurveColor = Color(255,0,0);
	mLod = 0.001f;
	int dx = (gWidth - 20) / (_n + 2) ;
	int y = gHeight /2;

	
	for(int i = 0; i<_n+1; i++) // n+1个控制点
	{
		mCtrlPoints.push_back( vec2i(dx * i + 20,y) );
	}
}


BezierCurve::~BezierCurve()
{
}

void BezierCurve::DrawCurve()
{

	float iter = 0.0f;
	while(iter < 1.0f)
	{
		vec2i dd = BezierCurveSum(iter);
		if(dd.x<gWidth && dd.y<gHeight && dd.x>=0&&dd.y>=0)
		{
			Platform::SetPixel(dd.x,dd.y,mCurveColor);
		}
		iter += mLod;
	}

	for(int i = 0; i<nTimes; i++)
	{
		Rasterize::DrawLine(mCtrlPoints[i],mCtrlPoints[i+1],mlineColor);
	}

	for(int i = 0; i<nTimes +1; i++)
	{
		Rasterize::DrawBox(mCtrlPoints[i],2,mBoxColor);
	}
}

void BezierCurve::SetCtrlPoint(const vec2i &pos,int i)
{
	if(pos.x<gWidth && pos.y<gHeight && pos.x>=0&&pos.y>=0)
	{
		mCtrlPoints[i] = pos;
	}
	Platform::PrintHelpCurve();
}

void BezierCurve::ResetCtrlPoint()
{
	Update();
}

std::string BezierCurve::GetControlPointsString()
{
	auto out = "Control Points Num: ";
	std::stringstream ss;
	ss<<out<<nTimes+1<<std::endl;
	for(int i = 0; i<nTimes+1; i++)
	{
		ss<<"Point "<<i<<": ["<<mCtrlPoints[i].x<<","<< mCtrlPoints[i].y<<"]"<<std::endl;
	}

	return ss.str();
}

int BezierCurve::FindNearestPoint(const vec2i & a)
{
	float len = 100000.0f;
	int index = 0;
	for(int i = 0; i<nTimes+1; i++)
	{
		vec2i re = mCtrlPoints[i] - a;
		float tmp = (float)(sqrt(re.x * re.x+re.y *re.y));
		if(len> tmp)
		{
			len = tmp;
			index = i;
		}
		
	}

	if(len > 6.0f)
		return-1;

	return index;
}

int BezierCurve::Factorial(int n)
{
	if(n == 0) return 1;
	int factorial = 1;
	for(int i = 1; i <=n; ++i)
	{
		factorial *= i;
	}
	return factorial;
}

float BezierCurve::Bernstein(float u,int index)
{
	float factor_0 = Factorial(nTimes) / (Factorial(nTimes - index) * Factorial(index));
	float factor_1 = pow((1 - u),(nTimes - index));
	float factor_3 = pow(u,index);
	return  factor_0 * factor_1 * factor_3;
}

vec2i BezierCurve::BezierCurveSum(float u)
{
	float x_result = 0;
	float y_result = 0;
	for(int i = 0; i<nTimes +1; i++) //n+1个控制点
	{
		x_result += Bernstein(u,i) * mCtrlPoints[i].x;
		y_result += Bernstein(u,i) * mCtrlPoints[i].y;
	}
	return vec2i(x_result,y_result);
}

void BezierCurve::Update()
{
	int dx = (gWidth - 20) / (nTimes + 2) ;
	int y = gHeight /2;
	mCtrlPoints.clear();
	for(int i = 0; i<nTimes+1; i++) // n+1个控制点
	{
		mCtrlPoints.push_back( vec2i(dx * i + 20,y) );
	}
	Platform::PrintHelpCurve();
}


