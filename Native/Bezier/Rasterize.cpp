#include "Rasterize.hpp"




void Rasterize::DrawLine(const vec2i & p0,const vec2i & p1,const Color & col)
{
	int x0 = p0.x; 
	int x1 = p1.x;
	int y0 = p0.y; 
	int y1 = p1.y;
	bool steep = false;
	if (std::abs( x0 - x1 ) < std::abs( y0 - y1 ))
	{
		std::swap( x0 , y0 );
		std::swap( x1 , y1 );
		steep = true;
	}

	if (x0 > x1)
	{
		std::swap( x0 , x1 );
		std::swap( y0 , y1 );
	}


	int dx = x1 - x0;
	int dy = y1 - y0;


	int dy2 = std::abs( dy ) * 2;
	int d = 0;
	int y = y0;


	for (int x = x0; x <= x1; x++)
	{
		if (steep)
		{
			Platform::SetPixel( y , x , col );
		}
		else
		{
			Platform::SetPixel( x , y , col );
		}

		d += dy2;
		if (d > dx)
		{
			y += ( y1 > y0 ? 1 : -1 );
			d -= dx * 2;
		}
	}
}

void Rasterize::DrawBox(const vec2i & pos,const int size,const Color & col)
{
	int x = pos.x - size;
	int y = pos.y - size;
	int tmp = 2*size+1;
	for(int i = 0; i<tmp; i++)
	{
		for(int j = 0; j<tmp; j++)
		{
			if(x<gWidth && y<gHeight && x>=0&&y>=0)
			{
				Platform::SetPixel(x,y,col);
			}
			x += 1;
		}
		y += 1;
		x = pos.x - size;
	}
}

void Rasterize::RasterizeTriangle(vec4f SV_vertexs[3],Shader & shader)


{
	// 1. 视口转换 (-1,1) => (0,width/height)
	vec3f gl_coord[3];

	//用于透视插值校正
	float re_w[3]; 
	for(int i = 0; i<3; i++)
	{
		re_w[i] = SV_vertexs[i].w;
		gl_coord[i].x = (SV_vertexs[i].x + 1.0f) * gWidth / 2.f;
		gl_coord[i].y = (SV_vertexs[i].y + 1.0f) * gHeight / 2.f;
		gl_coord[i].z = (SV_vertexs[i].z + 1.0f) / 2.f; 
	}

	// 2.计算包围盒 
	float xMax = (std::max)({gl_coord[0].x, gl_coord[1].x, gl_coord[2].x});
	float xMin = (std::min)({gl_coord[0].x, gl_coord[1].x, gl_coord[2].x});

	float yMax = (std::max)({gl_coord[0].y, gl_coord[1].y, gl_coord[2].y});
	float yMin = (std::min)({gl_coord[0].y, gl_coord[1].y, gl_coord[2].y});

	xMax = (std::min)(xMax,(float)(gWidth -1.f));
	xMin = (std::max)(xMin, 0.f);
	yMax = (std::min)(yMax, (float)(gHeight -1.f));
	yMin = (std::max)(yMin, 0.f);

	int x = 0;
	int y =0;
	//遍历包围盒内的像素点
	for(x = xMin; x<= xMax; x++)
	{
		for(y = yMin; y<= yMax; y++)
		{
			//计算重心三角形
			vec2f current_pixel = vec2f(x,y);
			vec2f A = vec2f(gl_coord[0].x,gl_coord[0].y);
			vec2f B = vec2f(gl_coord[1].x,gl_coord[1].y);
			vec2f C = vec2f(gl_coord[2].x,gl_coord[2].y);
			vec3f weight = Barycentric(A,B,C,current_pixel);

			//不在三角形内部的像素点跳过
			if(weight.x <0 || weight.y<0||weight.z < 0) continue;

			//深度插值 因为没有透视校正所以为非线性
			float currentDepth = weight.x * gl_coord[0].z+
				weight.y * gl_coord[1].z+
				weight.z * gl_coord[2].z;

			//深度测试
			if(currentDepth > gBezierSurface.mZbuffer[y * gWidth+x]) continue;

			//深度排序
			gBezierSurface.mZbuffer[y * gWidth+x] = currentDepth;

			//透视校正插值
			float weight0 = re_w[0] * weight.x;
			float weight1 = re_w[1] * weight.y;
			float weight2 = re_w[2] * weight.z;
			vec3f lerpWeight = vec3f(weight0,weight1,weight2);

			Color fragmenCol = shader.fragment(lerpWeight);
			Platform::SetPixel(x,y,fragmenCol);
		}
	}

}

vec3f Rasterize::Barycentric(vec2f A,vec2f B,vec2f C,vec2f P)
{
	vec2f ab = B - A;
	vec2f ac = C - A;
	vec2f ap = P - A;
	float factor = 1 / (ab.x * ac.y - ab.y * ac.x);
	float s = (ac.y * ap.x - ac.x * ap.y) * factor;
	float t = (ab.x * ap.y - ab.y * ap.x) * factor;
	vec3f weights = vec3f(1 - s - t, s, t);
	return weights;
}