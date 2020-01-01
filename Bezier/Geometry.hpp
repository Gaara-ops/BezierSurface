#pragma once
#include "Maths.hpp"

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



namespace Geometry
{
	mat ComputeModelMatrix(const SenceParameters&);
	mat ComputeViewMatrix(const SenceParameters&);
	mat ComputeProjectMatrix(const SenceParameters&);	
	mat ComputeViewportMatrix(float,float,float,float,int,int);

	mat ComputeMVP(const SenceParameters&);

}

