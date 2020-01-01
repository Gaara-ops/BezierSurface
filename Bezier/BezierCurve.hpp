#pragma once
#include "Maths.hpp"
#include <string>
class BezierCurve
{
public:
	BezierCurve(){ }
	BezierCurve(int);
	~BezierCurve();
	void DrawCurve();
	int FindNearestPoint(const vec2i&);
	void SetCtrlPoint(const vec2i&,int);
	void ResetCtrlPoint();
	std::string GetControlPointsString();
	int nTimes;//±´Èû¶ûÇúÏß´ÎÊý
	
	
private:
	int Factorial(int n);
	float Bernstein(float u,int index);
	vec2i BezierCurveSum(float u);

	void Update();

	vvec2i mCtrlPoints;
	Color mlineColor;
	Color mCurveColor;
	Color mBoxColor;
	
	float mLod;
};

