#pragma once
#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class Pixy
{
public:
	Pixy();
	Pixy(const std::string& file_name);
	Pixy(const std::string& file_name, const int &pixel_size);
	~Pixy();

	void ConvertImageToPixels(const int &pixel_size);
	void SetColorCount(const int &color_count);
	void GetFullColorPixelArt(const int & max_range);

	void ShowImage();
	void SaveImage(string file_name);

	int GetPixelSize();

private:
	void SetBlockColor(int x, int y, const Vec3b& color);
	void ConvertBlockToPixel(const int & i, const int & j);
	Vec3b GetNearestColor(std::map<int, std::vector<uchar>>& color_map, Vec3b& color);
	Vec3b ConvertVectorToVec(const std::vector<uchar> & vector);
	std::map<std::vector<uchar>, int> GetColorMap();

	Mat _image;
	int _height;
	int _width;
	int _pixel_size = 16;
};