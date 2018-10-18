#include "stdafx.h"
#include "Pixy.hpp"

#define CAR "car.jpg"
#define GIRL "girl.jpg"
#define COAL "coal.jpg"
#define MOUNTAINS "mounts.jpg"
#define LEAF "leaf.jpg"

int _tmain(int argc, _TCHAR* argv[])
{
	Pixy pixy(MOUNTAINS);

	pixy.ConvertImageToPixels(8);
	
	
	pixy.GetFullColorPixelArt(1000);
	pixy.ShowImage();
	pixy.SaveImage("mounts2.jpg");
	return 0;
}

