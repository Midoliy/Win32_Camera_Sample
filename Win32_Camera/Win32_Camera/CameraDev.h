#pragma once
#include <vector>

namespace camera 
{
	// カメラデバイスを管理するクラス
	class CameraDev
	{
	/*private:
		TCHAR name[256];
		std::vector<Resolution> res;*/

	public:
		CameraDev();
		~CameraDev();
		/*PTCHAR					GetName();
		std::vector<Resolution> GetResolution();*/

	};

	// 解像度を保存する値型
	struct Resolution
	{
		int	   width;
		int    height;
		double frame;
	};
}