#include "Renderer.hpp"
#include "Rasterize.hpp"
#include "BezierCurve.hpp"
#include "BezierSurface.hpp"
#include "Shader.hpp"
#include "Geometry.hpp"
#include <iostream>
bool gIsCurve = true;


bool Renderer::Init()
{
	const char* winTitle = "����������/����";
	Platform::MyRegisterClass();
	if(!Platform::InitInstance(mWidth,mHeight,winTitle))
	{
		return false;
	}
	return true;
}

void Renderer::Loop()
{
	
	if(gIsCurve)	//����������
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
		
		gBezierSurface = BezierSurface("fix50x50.obj",4);
		PhongShader shader_phong = PhongShader();
		MSG msg;
		gBanOutput = false;
		Platform::PrintHelpSurface();
		gBanOutput = true;
		SenceReset(); 
		while (true)
		{
			if (PeekMessage(&msg,0,0,0,PM_REMOVE))
			{
				if(msg.message == WM_QUIT) 
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			gBezierSurface.ClearZbuffer();
			memset(gFbo, 0, mWidth * mHeight * 4);
			gBezierSurface.InputManage();
			shader_phong.MVP = Geometry::ComputeMVP();
			gMVP = shader_phong.MVP;

			for(int i = 0; i<gBezierSurface.mMesh.NumFaces; i++)
			{
				vec3f triangleVertex[3];
	

				for(int j = 0; j<3; j++)
				{
					triangleVertex[j] = gBezierSurface.mMesh.vertexs[ gBezierSurface.mMesh.faceVertexIndex[i].raw[j] -1];
				}

				//����������ת��
				for(int j = 0; j<3; j++)
				{
					triangleVertex[j] = gBezierSurface.BezierSurfaceTansform(triangleVertex[j]);
				}

				//���¼��㷨��
				vec3f _mNormal = ((triangleVertex[1] - triangleVertex[0])^(triangleVertex[2] - triangleVertex[0])).normalize();


				vec4f SV_Vertex[3];
				for(int j = 0; j < 3; ++j){
					SV_Vertex[j] = shader_phong.vertex(triangleVertex[j],_mNormal,j,gLight);
				}


				//͸�ӳ����׶�
				for(int j = 0; j < 3; ++j){
					float re_w = 1.0f / SV_Vertex[j].w;
					SV_Vertex[j] = SV_Vertex[j] * re_w;
					SV_Vertex[j].w = re_w;
				}


				Rasterize::RasterizeTriangle(SV_Vertex, shader_phong);
			}

			// 3�α��������� ���Ƶ���Ⱦ
			float Min_len = 10000.f;
			for(int i = 0; i<4; i++)
			{
				for(int j = 0; j<4; j++)
				{
					Color RenderColor = Color(255,255,0);
					vec3f t_point = gBezierSurface.controlPoints[i][j];
					//MVP�任
					vec4f result =  (shader_phong.MVP * mat(t_point)).ToVec4f();
					//͸�ӳ���
					result = result * (1/result.w);
					//��Ļ����ת��
					int screen_coord_x = (result.x + 1.0f)* gWidth / 2.f;
					int screen_coord_y = (result.y + 1.0f)*gHeight / 2.f;
					float screen_coord_z = (result.z + 1.0f) / 2.f;

					
					//Խ�����
					if(screen_coord_y * gWidth+screen_coord_x > gWidth * gHeight -2) continue;
					if(screen_coord_y > gHeight -1 || screen_coord_y < 0) continue;
					if(screen_coord_x > gWidth || screen_coord_x <0) continue;
					//��Ȳ���
					if(screen_coord_z > gBezierSurface.mZbuffer[screen_coord_y * gWidth+screen_coord_x]) continue;
					gBezierSurface.mZbuffer[screen_coord_y * gWidth+screen_coord_x] = screen_coord_z;

					//�����λ��
					if(gSurfaceCtrlPoint.x >= 0)
					{
						if(gSurfaceCtrlPoint.x==i&&gSurfaceCtrlPoint.y==j)
						{
							RenderColor = Color(255,0,0);
						}
					}
					Rasterize::DrawBox(vec2i(screen_coord_x,screen_coord_y),2,RenderColor);
				}
			}
			Platform::UpdateBuffer(mWidth,mHeight);
		}
	}
}






