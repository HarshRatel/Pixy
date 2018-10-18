#include "stdafx.h"

#include "Pixy.hpp"
#include "Utils.hpp"

int sum(int a, int b)
{
	return a + b;
}

Pixy::Pixy(){}

Pixy::Pixy(const std::string &file_name)
{
	_image = imread(file_name, CV_LOAD_IMAGE_ANYCOLOR);
	_height = _image.rows;
	_width = _image.cols;
}

Pixy::Pixy(const std::string& file_name, const int& pixel_size) : _pixel_size(pixel_size)
{
	_image = imread(file_name, CV_LOAD_IMAGE_ANYCOLOR);
}

Pixy::~Pixy() {}

void Pixy::ConvertImageToPixels(const int &pixel_size)
{
	_pixel_size = pixel_size;
	const int height = _image.rows - _pixel_size;
	const int width = _image.cols - _pixel_size;

	for (int x = 0; x < height; x += _pixel_size)
	{
		for (int y = 0; y < width; y += _pixel_size)
			ConvertBlockToPixel(x, y);
	}
}

void Pixy::SetColorCount(const int &color_count)
{
	std::map<std::vector<uchar>, int> color_map = GetColorMap();
	
	std::map<int, std::vector<uchar>> freq_map;

	for (auto value : color_map)
		freq_map[value.second] = value.first;

	std::map<int, std::vector<uchar>>::iterator iterator = freq_map.end();
	for (int i = 0; i < color_count || iterator != freq_map.begin(); ++i)
		--iterator;

	freq_map.erase(freq_map.begin(), iterator);
	int i = 0;

	//implement in another method
	for (size_t i = 0; i < _height; i += _pixel_size)
	{
		for (size_t j = 0; j < _width; j += _pixel_size)
		{
			Vec3b current_color = _image.at<Vec3b>(i, j);
			Vec3b nearest_color = GetNearestColor(freq_map, current_color);
			SetBlockColor(i, j, nearest_color);
			_image.at<Vec3b>(i, j).val[0] = nearest_color.val[0];
			_image.at<Vec3b>(i, j).val[1] = nearest_color.val[1];
			_image.at<Vec3b>(i, j).val[2] = nearest_color.val[2];
		}
	}
}

int Sqr(int a)
{
	return a * a;
}

vector<std::vector<uchar>> DeleteCloseColors(const std::map<int, std::vector<uchar>>& freq_map, const vector<uchar> color, const int & max_range)
{
	vector<std::vector<uchar>> result;

	for (auto i = --freq_map.end(); i != freq_map.begin(); --i)
	{		
		if (i == freq_map.end())
			continue;
		
		int metric = Sqr((int)i->second[0] - (int)color[0]) + Sqr((int)i->second[1] - (int)color[1]) + Sqr((int)i->second[2] - (int)color[2]);

		if (metric >= max_range)
		{
			result.push_back(i->second);
		}
	}

	return result;
}

//not finished yet
void Pixy::GetFullColorPixelArt(const int& max_range)
{
	std::map<std::vector<uchar>, int> color_map = GetColorMap();

	std::map<int, std::vector<uchar>> freq_map;

	for (auto value : color_map)
		freq_map[value.second] = value.first;

	vector<vector<uchar>> colors = GetValues<int, vector<uchar>>(freq_map);

	for ()
	for (size_t i = 0; i < _height; i += _pixel_size)
	{
		for (size_t j = 0; j < _width; j += _pixel_size)
		{
			Vec3b current_color = _image.at<Vec3b>(i, j);
			Vec3b nearest_color = GetNearestColor(freq_map, current_color);
			SetBlockColor(i, j, nearest_color);
			_image.at<Vec3b>(i, j).val[0] = nearest_color.val[0];
			_image.at<Vec3b>(i, j).val[1] = nearest_color.val[1];
			_image.at<Vec3b>(i, j).val[2] = nearest_color.val[2];
		}
	}
}

void Pixy::SetBlockColor(int x, int y, const Vec3b& color)
{
	for (int i = 0; i < _pixel_size; ++i)
	{
		for (int j = 0; j < _pixel_size; ++j)
			_image.at<Vec3b>(x + i, y + j) = color;
	}
}

void Pixy::ShowImage()
{
	try
	{
		namedWindow("Display window", WINDOW_AUTOSIZE);
		imshow("Display window", _image);
	}
	catch (...)
	{
		throw std::runtime_error("Failed to show image");
	}

	waitKey(0);
}

void Pixy::SaveImage(string file_name)
{
	imwrite(file_name, _image);
}

int Pixy::GetPixelSize()
{
	return _pixel_size;
}

void Pixy::ConvertBlockToPixel(const int& x, const int& y)
{
	Vec3b pixel_color;
	Vec3b resultColor;

	vector<int> intencity(3);

	for (int i = 0; i < _pixel_size; ++i)
	{
		for (int j = 0; j < _pixel_size; ++j)
		{
			pixel_color = _image.at<Vec3b>(x + i, y + i);
			intencity[0] += pixel_color.val[0];
			intencity[1] += pixel_color.val[1];
			intencity[2] += pixel_color.val[2];
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		intencity[i] /= _pixel_size * _pixel_size;
		resultColor.val[0] = intencity[0];
		resultColor.val[1] = intencity[1];
		resultColor.val[2] = intencity[2];
	}

	for (int i = 0; i < _pixel_size; ++i)
	{
		for (int j = 0; j < _pixel_size; ++j)
			_image.at<Vec3b>(x + i, y + j) = resultColor;
	}
}

Vec3b Pixy::GetNearestColor(std::map<int, std::vector<uchar>>& color_map, Vec3b &color)
{
	Vec3b nearest_color;
	int min_metric = 100000000;

	for (auto& value : color_map)
	{
		Vec3b check_color = ConvertVectorToVec(value.second);
		int metric = Sqr((int)value.second[0] - (int)color.val[0]) + Sqr((int)value.second[1] - (int)color.val[1]) + Sqr((int)value.second[2] - (int)color.val[2]);
		//double metric = norm(check_color, color., CV_L2);

		if (min_metric != min<int>(min_metric, metric))
		{
			min_metric = min<int>(min_metric, metric);
			nearest_color = ConvertVectorToVec(value.second);
		}
	}

	return nearest_color;
}

Vec3b Pixy::ConvertVectorToVec(const std::vector<uchar>& vector)
{
	if (vector.size() < 3)
		throw std::runtime_error("Wrong vector size");

	Vec3b vec;
	vec.val[0] = vector[0];
	vec.val[1] = vector[1];
	vec.val[2] = vector[2];

	return vec;
}

std::map<std::vector<uchar>, int> Pixy::GetColorMap()
{
	std::map<std::vector<uchar>, int> color_map;

	for (size_t i = 0; i < _height; i += _pixel_size)
	{
		for (size_t j = 0; j < _width; j += _pixel_size)
		{
			Vec3b color = _image.at<Vec3b>(i, j);
			vector<uchar> temp = { color.val[0], color.val[1], color.val[2] };
			++color_map[temp];
		}
	}

	return color_map;
}
