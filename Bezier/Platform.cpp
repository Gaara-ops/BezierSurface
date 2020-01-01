#include "Platform.hpp"
#include "Renderer.hpp"

HWND ghMainWnd		= NULL;				
HDC ghdcMainWnd     = NULL;			
HBITMAP gdibMap		= NULL;		
HBITMAP goldMap		= NULL;	
int gWidth			= 0;
int gHeight			= 0;
unsigned char* gFbo  = nullptr;
int gInputKeys[512] = {0};	
BezierCurve gBezierCurve;
int gMouseCtrlPoint = -1;

void Platform::MyRegisterClass()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "WinDOWCLASSNAME";
	RegisterClass(&wc);
}

BOOL Platform::InitInstance(int width,int height,const char* title)
{
	gWidth = width;
	gHeight = height;
	ghMainWnd = CreateWindow("WinDOWCLASSNAME", title, 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), NULL);

	if (ghMainWnd == NULL)
	{
		return FALSE;
	}


	HDC tmpHdc = GetDC(ghMainWnd);
	ghdcMainWnd = CreateCompatibleDC(tmpHdc);
	ReleaseDC(ghMainWnd, tmpHdc);

	// 创建DIB
	LPVOID ptr;
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), 
		width, -height, 1, 32, BI_RGB, 
		width * height * 4, 0, 0, 0, 0 }  };
	gdibMap = CreateDIBSection(ghdcMainWnd, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if(gdibMap == NULL)
		return false;
	goldMap = (HBITMAP)SelectObject(ghdcMainWnd, gdibMap);

	// 创建并绑定帧缓存
	gFbo = (unsigned char*)ptr;
	memset(gFbo, 0, width * height * 4);
	memset(gInputKeys, 0, sizeof(int) * 512);

	// 调整窗口大小和位置
	RECT rect = { 0, 0, width, height };
	AdjustWindowRect(&rect, GetWindowLong(ghMainWnd, GWL_STYLE), 0);
	int wx = rect.right - rect.left;
	int wy = rect.bottom - rect.top;
	int sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	int sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;

	SetWindowPos(ghMainWnd, NULL, sx, sy, wx, wy, 
		(SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(ghMainWnd);

	ShowWindow(ghMainWnd, SW_SHOW);
	UpdateWindow(ghMainWnd);


	return TRUE;
}

void Platform::UpdateBuffer(int& gWidth,int& gHeight)
{
	HDC hDC = GetDC(ghMainWnd);
	BitBlt(hDC, 0, 0, gWidth, gHeight, ghdcMainWnd, 0, 0, SRCCOPY);
	ReleaseDC(ghMainWnd, hDC);
}

LRESULT CALLBACK Platform::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_LBUTTONDOWN:
	{
		int mouse_x = (int)LOWORD(lParam);
		int mouse_y = (int)HIWORD(lParam);
		vec2i mPos = vec2i(mouse_x,mouse_y);

		if(gInputKeys[VK_LBUTTON&511]==0)
		{
			int nearPoint = gBezierCurve.FindNearestPoint(mPos);
			if(nearPoint < 0) return 0;
			gMouseCtrlPoint = nearPoint;
			gBezierCurve.SetCtrlPoint(mPos,nearPoint);
		}

		gInputKeys[VK_LBUTTON & 511] = 1;
	}break;
	case WM_LBUTTONUP:
	{
		gMouseCtrlPoint = -1;
		gInputKeys[VK_LBUTTON & 511] = 0;
	}break;

	case WM_MOUSEMOVE:
	{
		if(gIsCurve)//贝塞尔曲线
		{

			int mouse_x = (int)LOWORD(lParam);
			int mouse_y = (int)HIWORD(lParam);

			vec2i mPos = vec2i(mouse_x,mouse_y);
			int buttons = (int)wParam;
			if(buttons & MK_LBUTTON)
			{
				if(gMouseCtrlPoint>=0)
				{
					gBezierCurve.SetCtrlPoint(mPos,gMouseCtrlPoint);
				}
			}
		}
	}break;

	

		// 按键按下
	case WM_KEYDOWN:
	{
		
		
		// 退出程序
		if(wParam==VK_ESCAPE)
		{
			DestroyWindow(ghMainWnd);
		}

		if(wParam==VK_UP)
		{
			if(gBezierCurve.nTimes<=10&&gInputKeys[VK_UP]==0&&gIsCurve)
			{
				gBezierCurve.nTimes += 1;
				gBezierCurve.ResetCtrlPoint();
			}
				
		}

		if(wParam==VK_DOWN)
		{
			if(gBezierCurve.nTimes>=4&&gInputKeys[VK_DOWN]==0&&gIsCurve)
			{
				gBezierCurve.nTimes -= 1;
				gBezierCurve.ResetCtrlPoint();
			}
				
		}

		//设置为按下状态
		gInputKeys[wParam & 511] = 1;
	}
	break;

	case WM_KEYUP:
	{

		//设置为松开状态
		gInputKeys[wParam & 511] = 0;
	}
	break;
	// 处理退出
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		// 默认
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void Platform::PrintHelpCurve()
{

	system("cls");
	std::cout<<"=========  user tips  =========="<<std::endl;
	std::cout<<"   1. space  ：switch curve/surface.  "<<std::endl;
	std::cout<<"   2. ↑ up   ：add control point.  "<<std::endl;
	std::cout<<"   3. ↓ down ：less control points.  "<<std::endl;
	std::cout<<std::endl;
	std::cout<<"=========  Bezier Curve  =========="<<std::endl;

	std::cout<<gBezierCurve.GetControlPointsString();
}





void Platform::SetPixel(const int & x,const int & y,const Color & col)
{
	if(x<gWidth && y<gHeight && x>=0&&y>=0)
	{
		unsigned char *dst = &(gFbo[y * gWidth * 4 + x*4]);
		dst[0] = (unsigned char)(col.b); 
		dst[1] = (unsigned char)(col.g);  
		dst[2] = (unsigned char)(col.r);
		dst = nullptr;
	}
}