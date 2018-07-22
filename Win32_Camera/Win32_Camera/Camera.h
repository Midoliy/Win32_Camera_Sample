#pragma once
#include <tchar.h>
#include <vector>

// 解像度を保存する値型
struct Res
{
	int	   width;
	int    height;
	double frame;
};

// カメラを保存する値型
struct Camera
{
	TCHAR			 name[256];
	std::vector<Res> res;
};