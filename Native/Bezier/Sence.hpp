#pragma once
#include "Maths.hpp"
#include "Platform.hpp"

//����
extern vec3f gMeshMove;	 // ģ��������ռ���ƫ��
extern vec3f gMeshRotate;	 // ģ����תֵ
extern vec3f gMeshScale;	 // ģ������ֵ
extern bool gBanOutput;
							 //�ƹ�����
extern vec3f gLight;		//�ƹ�λ��
extern mat gMVP;
							// �������
extern vec3f gEye;			//���������ռ��µ�λ��
extern vec3f gAt;			//����ӵ�
extern vec3f gUp;			//������Ϸ�������
extern float gAspect;		//��߱�
extern float gFovy;		//��Ұ
extern float gFarZ;		//��ƽ��
extern float gNearZ;		//Զƽ��

std::string SenceParameterOutPut();
void SenceReset();