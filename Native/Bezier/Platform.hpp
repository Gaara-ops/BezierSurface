#pragma once
#include <Windows.h>
#include "Maths.hpp"
#include "BezierCurve.hpp"
#include "BezierSurface.hpp"

struct SenceParameters
{
	//ģ������
	vec3f gMeshMove;	 // ģ��������ռ���ƫ��
	vec3f gMeshRotate;	 // ģ����תֵ
	vec3f gMeshScale;	 // ģ������ֵ

						 //�ƹ�����
	vec3f gLight;		//�ƹ�λ��

						// �������
	vec3f gEye;			//���������ռ��µ�λ��
	vec3f gAt;			//����ӵ�
	vec3f gUp;			//������Ϸ�������
	float gAspect;		//��߱�
	float gFovy;		//��Ұ
	float gFarZ;		//��ƽ��
	float gNearZ;		//Զƽ��
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

