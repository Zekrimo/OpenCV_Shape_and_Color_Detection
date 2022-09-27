#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <cmath>
#include <iostream>

#include "ShapeAndColorDetection.h"

//namespaces
using namespace cv;
using namespace std;

int main(int argc, char **argv)
{

	ShapeAndColorDetection detector;
	detector.parse(argc, argv);

	while (true)
	{
		//iterate frame
		detector.detect();


	}



}

