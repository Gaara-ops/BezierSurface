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
