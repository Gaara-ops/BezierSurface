#pragma once
#include "Maths.hpp"
#include "Platform.hpp"

//场景
extern vec3f gMeshMove;	 // 模型在世界空间下偏移
extern vec3f gMeshRotate;	 // 模型旋转值
extern vec3f gMeshScale;	 // 模型缩放值
extern bool gBanOutput;
							 //灯光属性
extern vec3f gLight;		//灯光位置
extern mat gMVP;
							// 相机属性
extern vec3f gEye;			//相机在世界空间下的位置
extern vec3f gAt;			//相机视点
extern vec3f gUp;			//相机向上方向向量
extern float gAspect;		//宽高比
extern float gFovy;		//视野
extern float gFarZ;		//近平面
extern float gNearZ;		//远平面

std::string SenceParameterOutPut();
void SenceReset();