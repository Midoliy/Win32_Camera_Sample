#pragma once
#include <vector>
#include <memory>

namespace camera 
{
	// カメラデバイスを管理するクラス
	class CameraDev
	{
	private:
		std::wstring name;
		std::vector<Resolution> resolutions;
		std::unique_ptr<ICreateDevEnum> createDevEnum();
		CComPtr<IMoniker> moniker;

	public:
		CameraDev();
		~CameraDev();
		std::wstring			GetName();
		std::vector<Resolution> GetResolution();

	};

	// 解像度を保存する値型
	struct Resolution
	{
		int	   width;
		int    height;
		double frame;
	};

}