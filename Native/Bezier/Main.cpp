#include "Renderer.hpp"

int main()
{
	std::cout<<"++++++++++++++++++++++++++++++++++++++++"<<std::endl;
	std::cout<<" ����������/������� "<<std::endl;
	std::cout<< " ���봰�ڿ�int�����Ƽ�500 �� "<<std::endl;
	int width = 0;
	std::cin>>width;
	std::cout<< " ���봰�ڸߣ�int�����Ƽ�500 �� "<<std::endl;
	int height = 0;
	std::cin>>height;
	std::cout<< " ѡ������/���ߣ�int��  -- ����1�������ߣ�����2 �������� "<<std::endl;
	int mode = -1;
	std::cin>>mode;


	Renderer RTX = Renderer(width,height,mode == 1);
	RTX.Init();
	RTX.Loop();
}
