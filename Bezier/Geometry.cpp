#include "Geometry.hpp"


//
//  ����: ComputeModelMatrix(const mat& scale,const mat& rotate,const mat& move)
//
//  Ŀ��: ����ģ�Ϳռ䵽����ռ��ת������
//
//    scale	   -  (Sx,Sy,Sz)  x,y,z ������ű���		
//    rotate   -  (Rx,Ry,Rz)  x,y,z �����ת����			
//    move     -  (Dx,Dy,Dz)  x,y,z ���ԭ��ƫ��	
//
mat Geometry::ComputeModelMatrix(const SenceParameters& m)
{
	/*
	�������ţ���ʾΪ
	����							 ����				����
	|	x_World = k1 * x_Model;		 |  k1  0	0	 |
	S =   | y_World = k2 * y_Model;  =�� |  0	k2	0	 |
	| z_World = k3 * z_Model;		 |  0   0	k3	 |
	����							 ����				����

	λ�ƣ���ʾΪ


	����							 ����				  ����
	|	x_World = d1 + x_Model;		 |  1   0	0	d1 |
	M =   | y_World = d2 + y_Model;  =�� |  0	1	0	d2 |
	| z_World = d3 + z_Model;		 |  0   0	1   d3 |
	|								 |	0   0   0   1  |
	����							 ����				 ����

	��ת��������x-y����ϵ�£��߶γ���Ϊr���������ԭ�㣬�յ�Ϊp1,��x��н�Ϊ a1,
	��ת��Ϊa2,�յ�Ϊp2������ת��Ϊԭ�㡣

	��ô: �� p1����Ϊ (x1,y1)
	p2 ����Ϊ(x2,y2)

	x1 = r* cosa1;
	y1 = r* sina1;

	x2 = r*cos(a1+a2);
	y2 = r*sin(a1+a2);

	չ���У�
	x2 = x1* cosa2 - y1 * sina2;
	y2 = x1* sina2 + y1 * cosa2;

	ʵ���ϣ�z�����Ϊ��ֱ��x-yƬ��ָ���⣬��ˣ��������z�����ת���̡�
	ͬ���� x-y �滻�� x-z �� y-z���ɵõ� ��y����� x�����ת���̡�

	��ȡ���� Rx,Ry,Rz.
	��ô��ת�����Ϊ

	R = RzRyRx;		
	������ŷ�����������

	��ʱ��ģ�;���	Model =  Move * Rotate * Scale ����Ϊ��ξ���

	ע����淨�ߵ�ת��
	�������˷Ǿ������Ų�������ô����ת������
	Ӧ��ΪModel���ת�á�
	֤���� https://zhuanlan.zhihu.com/p/72734738 ����д��
	*/
	// 1.�������ž���
	mat ScaleMatrix = mat::identity(4);
	ScaleMatrix[0][0] = m.gMeshScale.x;
	ScaleMatrix[1][1] = m.gMeshScale.y;
	ScaleMatrix[2][2] = m.gMeshScale.z;

	// 2.����λ�ƾ���
	mat MoveMatrix = mat::identity(4);
	MoveMatrix[0][3] = m.gMeshMove.x;
	MoveMatrix[1][3] = m.gMeshMove.y;
	MoveMatrix[2][3] = m.gMeshMove.z;

	// 3.������ת����
	mat RotateMatrix = mat::identity(4);

	float cosX = std::cos(m.gMeshRotate.x * RtoPi);
	float sinX = std::sin(m.gMeshRotate.x * RtoPi);

	float cosY = std::cos(m.gMeshRotate.y *RtoPi);
	float sinY = std::sin(m.gMeshRotate.y * RtoPi);

	float cosZ = std::cos(m.gMeshRotate.z * RtoPi);
	float sinZ = std::sin(m.gMeshRotate.z * RtoPi);

	RotateMatrix[0][0] = cosY*cosZ;
	RotateMatrix[0][1] = -cosX*sinZ + sinX*sinY*cosZ;
	RotateMatrix[0][2] = sinX*sinZ + cosX*sinY*cosZ;

	RotateMatrix[1][0] = cosY*sinZ;
	RotateMatrix[1][1] = cosX*cosZ + sinX*sinY*sinZ;
	RotateMatrix[1][2] = -sinX*cosZ + cosX*sinY*sinZ;

	RotateMatrix[2][0] = -sinY;
	RotateMatrix[2][1] = sinX*cosY;
	RotateMatrix[2][2] = cosX*cosY;

	return MoveMatrix * RotateMatrix * ScaleMatrix;
}

//
//  ����: ComputeViewMatrix(const vec3f&,const vec3f&,const vec3f)
//
//  Ŀ��: �����Ӽ�ת������
//
//    eye	 -  (Ex,Ey,Ez)  �����������ռ��µ�����	
//    at	 -  (Ax,Ay,Az)  �����ָ��Ĳο���		
//    up     -  (Dx,Dy,Dz)  ����Ϸ�������ͨ������Ϊy��
//
//	��ӦOpenGL��LookAt����
//  �������ҵ���������
//
mat Geometry::ComputeViewMatrix(const SenceParameters& m)
{
	// 1.ʩ����������
	vec3f n = (m.gEye - m.gAt).normalize();  //z
	vec3f u = (m.gUp ^ n).normalize();	   //x
	vec3f v = (n ^ u).normalize();	   //y

	mat viewMat = mat::identity(4);
	viewMat[0][0] = u.x;
	viewMat[0][1] = u.y;
	viewMat[0][2] = u.z;
	viewMat[0][3] =  -(m.gEye * u);

	viewMat[1][0] = v.x;
	viewMat[1][1] = v.y;
	viewMat[1][2] = v.z;
	viewMat[1][3] =  -(m.gEye * v);

	viewMat[2][0] = n.x;
	viewMat[2][1] = n.y;
	viewMat[2][2] = n.z;
	viewMat[2][3] =  -(m.gEye * n);

	return viewMat;
}


//
//  ����: ComputeProjectMatrix(float,float,float,float)
//
//  Ŀ��: ����ͶӰ�������
//
//    fovy		-  ��ƽ�� y-z ����׶�����߼нǣ��Ƕȣ�	
//    aspect	-  ͶӰƽ���߱�	
//    farZ      -  ��׶����ԶZƽ����ԭ�����
//	  nearZ		-  ��׶�����Zƽ����ԭ�����
//
mat Geometry::ComputeProjectMatrix(const SenceParameters& m)
{
	mat projMat = mat::identity(4);
	// cot(fovy* pi / 360)
	float cotFovyDiv2 = 1.f / tan(m.gFovy*RtoPi / 2.f);

	//ʹ����������ϵ
	projMat[0][0] = cotFovyDiv2 / m.gAspect;
	projMat[1][1] = cotFovyDiv2;
	projMat[2][2] = m.gFarZ / (m.gFarZ - m.gNearZ);
	projMat[2][3] = m.gNearZ*m.gFarZ / (m.gFarZ - m.gNearZ);
	projMat[3][2] = -1;
	projMat[3][3] = 0;

	return projMat;
}


//
//  ����: ComputeViewportMatrix(float,float,float,float)
//
//  Ŀ��: �����ӿڱ任����
//
//    minX		-  ��Ļ���Ͻ�����Ⱦ�������ϽǶ����x����
//    minY		-  ��Ļ���Ͻ�����Ⱦ�������ϽǶ����y����
//    minZ      -  NDC���κ���СZ��� ��0��
//	  maxZ		-  NDC���κ����Z��� ��1��
//    gwidth/gheight	��Ļ���
//
mat Geometry::ComputeViewportMatrix(float minX,float minY,float maxZ,float minZ,int gWidth,int gHeight)
{
	mat viewport = mat::identity(4);
	viewport[0][0] = gWidth/2.f;
	viewport[1][1] = gHeight/2.f;
	viewport[2][2] = maxZ - minZ;
	viewport[0][3] = minX + gWidth/2;
	viewport[1][3] = minY + gHeight/2;
	viewport[2][3] = minZ;
	return viewport;
}



// ����MVP����
mat Geometry::ComputeMVP(const SenceParameters& sence)
{
	mat M = ComputeModelMatrix(sence);
	mat V = ComputeViewMatrix(sence);
	mat P = ComputeProjectMatrix(sence);

	return P * V * M;
}

