#include "Renderer.hpp"

int main()
{
	std::cout<<"++++++++++++++++++++++++++++++++++++++++"<<std::endl;
	std::cout<<" 贝塞尔曲线/曲面软件 "<<std::endl;
	std::cout<< " 输入窗口宽（int），推荐500 ： "<<std::endl;
	int width = 0;
	std::cin>>width;
	std::cout<< " 输入窗口高（int），推荐500 ： "<<std::endl;
	int height = 0;
	std::cin>>height;
	std::cout<< " 选择曲面/曲线（int）  -- 输入1代表曲线，输入2 代表曲面 "<<std::endl;
	int mode = -1;
	std::cin>>mode;


	Renderer RTX = Renderer(width,height,mode == 1);
	RTX.Init();
	RTX.Loop();
}
