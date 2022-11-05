/*
 * ShapeAndColorDetection.cpp
 *
 *  Created on: Sep 14, 2022
 *      Author: son
 */

#include "ShapeAndColorDetection.h"

ShapeAndColorDetection::ShapeAndColorDetection() :
		lowGreen(Scalar(30.0, 30.5, 84.15)), highGreen(
				Scalar(78.0, 255.0, 255.0)), lowPink(
				Scalar(145.0, 130.75, 153.0)), highPink(
				Scalar(193.0, 255.0, 255.0)), lowOrange(
				Scalar(6.0, 125.5, 115.35)), highOrange(
				Scalar(19.99, 255.0, 255.0)), lowYellow(
				Scalar(19.0, 123.75, 135.25)), highYellow(
				Scalar(29.9, 255.0, 255.0)), specificColor("all"), specificShape(
				"all"), vid(0), videoEnabled(false), batchEnabled(false), clockTicks(
				0), objectHasBeenFound(false), skip(false)
{
}

ShapeAndColorDetection::~ShapeAndColorDetection()
{
}

void ShapeAndColorDetection::setLabel(cv::Mat &im, const std::string& label,
		std::vector<cv::Point> &contour)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Rect r = cv::boundingRect(contour);

	string Xcords = to_string(r.x + r.width / 2);
	string Ycords = to_string(r.y + r.height / 2);
	string surface = to_string(contourArea(contour));

	String Cords = ("X:" + Xcords + " Y:" + Ycords);

	cv::Size text = cv::getTextSize(Cords, fontface, scale, thickness,
			&baseline);

	cv::Point pt(r.x + ((r.width - text.width) / 2),
			r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline),
			pt + cv::Point(text.width, -text.height), CV_RGB(255, 255, 255),
			FILLED);

	logObject(label, surface, Xcords, Ycords);
	cv::putText(im, Cords, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);

	//logObject(label, contourArea(contour), (int)pt.x, (int)pt.y);
	return;
}

void ShapeAndColorDetection::convertimage()
{

	cvtColor(src.clone(), hsv, COLOR_BGR2HSV);

	//convert to gray scale
	cvtColor(src, gray, COLOR_BGR2GRAY);

	GaussianBlur(gray, gray, Size(9, 9), 2, 2);

	//find corners with canny algo
	Canny(gray, blackwhite, 0, 50, 3);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(blackwhite, blackwhite, kernel);

	mask = src.clone();
	dst = src.clone();
}

// double ShapeAndColorDetection::angle(cv::Point pt1, cv::Point pt2,
//		cv::Point pt0)
//{
//	double dx1 = pt1.x - pt0.x;
//	double dy1 = pt1.y - pt0.y;
//	double dx2 = pt2.x - pt0.x;
//	double dy2 = pt2.y - pt0.y;
//	return (dx1 * dx2 + dy1 * dy2)
//			/ sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
//}

void ShapeAndColorDetection::showImage()
{
	if (videoEnabled)
	{
		cv::imshow("mask", mask);
		objectFound();
		cv::imshow("dst", dst);
		waitKey(0);
	}
	else if (batchEnabled)
	{
//		cv::imshow("src", src);
//		cv::imshow("mask", mask);
//		cv::imshow("dst", dst);
//		waitKey(0);
	}
	else
	{
		objectFound();
		cv::imshow("src", src);
		cv::imshow("mask", mask);
		cv::imshow("dst", dst);
		waitKey(0);
		exit(0);
	}
}

void ShapeAndColorDetection::labelPolygon(std::vector<cv::Point> &approxInput,
		vector<vector<cv::Point> > &contoursInput, String colortype,
		String shape)
{
	for (uint64_t i = 0; i < contoursInput.size(); i++)
	{

		// Approximate contour with accuracy proportional
		// to the contour perimeter
		cv::approxPolyDP(Mat(contoursInput[i]), approxInput,
				arcLength(Mat(contoursInput[i]), true) * 0.02, true);

		// Skip small or non-convex objects
		if (fabs(cv::contourArea(contoursInput[i])) < 100
				|| !cv::isContourConvex(approxInput))
			continue;
		if (approxInput.size() == 3 && (shape == "driehoek" || shape == "all"))
		{
			setLabel(dst, colortype + " DRIEHOEK", contoursInput[i]); // Triangles
			drawContours(dst, contoursInput, i, Scalar(255, 0, 0), 3, 8);

			//logObject("DRIEHOEK", colortype, contourArea(contoursInput.at(i)));
		}
		else if (approxInput.size() >= 4 && approxInput.size() <= 6)
		{
			// Number of vertices of poly curve
			int vtc = approxInput.size();

			// Get the cosines of all corners
			vector<double> cos;
			for (int j = 2; j < vtc + 1; j++)
				cos.push_back(
						angle(approxInput[j % vtc], approxInput[j - 2],
								approxInput[j - 1]));

			// Sort ascending the cosine values
			std::sort(cos.begin(), cos.end());

			// Get the lowest and the highest cosine
			double mincos = cos.front();
			double maxcos = cos.back();

			// Use the degrees obtained above and the number of vertices
			// to determine the shape of the contour
			if (vtc == 4 && mincos >= -0.3 && maxcos <= 0.5
					&& (shape == "rechthoek" || shape == "vierkant"
							|| shape == "all"))
			{

				Rect vierkant = boundingRect(contoursInput[i]);
				float diff = ((float) vierkant.width / (float) vierkant.height);

				if ((diff >= 0.8 && diff <= 1.2)
						&& (shape == "vierkant" || shape == "rechthoek"
								|| shape == "all"))
				{
					if (shape == "vierkant" || shape == "all")
					{
						setLabel(dst, colortype + " VIERKANT",
								contoursInput[i]);
						drawContours(dst, contoursInput, i, Scalar(255, 0, 0),
								3, 8);
					}
					else
					{
						setLabel(dst, colortype + " RECHTHOEK",
								contoursInput[i]);
						drawContours(dst, contoursInput, i, Scalar(255, 0, 0),
								3, 8);
					}
				}
				else if (!(diff >= 0.8 && diff <= 1.2)
						&& (shape == "rechthoek" || shape == "all"))
				{
					setLabel(dst, colortype + " RECHTHOEK", contoursInput[i]);
					drawContours(dst, contoursInput, i, Scalar(255, 0, 0), 3,
							8);
				}
			}
			else if (vtc > 4 && (shape == "halve cirkel" || shape == "all"))
			{
				string tmp = to_string(vtc);

				setLabel(dst, colortype + " HALVE CIRKEL", contoursInput[i]);
				drawContours(dst, contoursInput, i, Scalar(255, 0, 0), 3, 8);

			}
		}
		else
		{
			labelCircle(approx, contoursInput, i, colortype, shape);
		}

	}
}

void ShapeAndColorDetection::parse(int argc, char **argv)
{

	//cmdline parameter keys
	cv::String keys = // "{@image |video| input image path}" input image is the first argument (positional)
			"{@shape  |all| shape to be found}"// optional find specific color
					"{@shape2 |all| shape to be found}"// optional find specific color
					"{@color  |all| color to be found}"// optional find specific shape
	;

	CommandLineParser parser(argc, argv, keys);

	string shape = parser.get<String>("@shape");
	string shape2;
	string color = parser.get<cv::String>("@shape2");

	if (shape == "halve")
	{
		shape2 = parser.get<cv::String>("@shape2");
		color = parser.get<cv::String>("@color");
	}

	if (color.find("#"))
	{
		color = color.substr(0, color.find("#"));

	}

	//batch
	if (shape != "all" && (shape.substr(shape.length() - 4) == ".txt"))
	{
		cout << "batch input selected." << endl;
		vid.read(src);
		videoEnabled = true;
		processBatchFile(shape);
		exit(0);
	}
	else
	{
		if (!vid.isOpened())
		{
			std::cerr << "camera not connected" << std::endl;
			exit(0);
		}
		vid.read(src);
		videoEnabled = true;
		std::cout << "video input selected." << std::endl;
	}

	//check for halve cirkel parameter
	if (shape == "halve")
	{
		shape = shape + " " + shape2;
	}

	//setter of shape
	if (!setShape(shape))
	{
		cout << "unsupported shape: '" << shape << "'" << endl
				<< "terminating program" << endl;
		exit(0);

	}

	//setter of color
	if (!setColor(color))
	{
		cout << "unsupported color: '" << color << "'" << endl
				<< "terminating program" << endl;
		exit(0);
	}
}

void ShapeAndColorDetection::video()
{
	if (vid.read(src))
	{
		convertimage();
		findColorsAndShapes();
		showImage();
		if (!vid.isOpened())
		{
			std::cout << "camera disconnected" << std::endl;
			exit(0);
		}
	}

}

void ShapeAndColorDetection::detect()
{
	clockTicks = 0;
	if (videoEnabled)
	{
		video();
	}
	else
	{
		findColorsAndShapes();
		showImage();
	}

}

void ShapeAndColorDetection::labelCircle(std::vector<cv::Point> &approxInput,
		vector<vector<cv::Point> > &contoursInput, int iterator,
		String colortype, String shape)
{

	if ((approxInput.size() > 4) && (shape == "cirkel" || shape == "all"))
	{

		drawContours(dst, contoursInput, iterator, Scalar(255, 0, 0), 3, 8);
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		cv::approxPolyDP(Mat(contoursInput[iterator]), approxInput,
				arcLength(Mat(contoursInput[iterator]), true) * 0.02, true);

		// Skip small or non-convex objects
		if ((fabs(cv::contourArea(contoursInput[iterator])) < 100
				|| !cv::isContourConvex(approxInput)))
		{
			return;
		}
		// Detect and label circles
		double area = cv::contourArea(contoursInput[iterator]);
		Rect r = cv::boundingRect(contoursInput[iterator]);
		int radius = r.width / 2;

		if (abs(1 - ((double) r.width / r.height)) <= 0.2
				&& abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
			setLabel(dst, colortype + " CIRKEL", contoursInput[iterator]);
	}

}

void ShapeAndColorDetection::findYellowShapes()
{
	inRange(hsv, lowYellow, highYellow, mask);

	findContours(mask.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	labelPolygon(approx, contours, "GEEL", specificShape);
}

void ShapeAndColorDetection::findGreenShapes()
{
	inRange(hsv, lowGreen, highGreen, mask);

	findContours(mask.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	labelPolygon(approx, contours, "GROEN", specificShape);

}

void ShapeAndColorDetection::findOrangeShapes()
{
	inRange(hsv, lowOrange, highOrange, mask);

	findContours(mask.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	labelPolygon(approx, contours, "ORANJE", specificShape);
}

void ShapeAndColorDetection::findColorsAndShapes()
{

	convertimage();
	int state = getColorIndex(specificColor);

	switch (state)
	{
	case 0:
		findPinkShapes();
		break;
	case 1:
		findOrangeShapes();
		break;
	case 2:
		findGreenShapes();
		break;
	case 3:
		findYellowShapes();
		break;
	case 4:
		findPinkShapes();
		findOrangeShapes();
		findGreenShapes();
		findYellowShapes();
		break;
	default:
		cout << "unsupported color given. program is terminated statemachine"
				<< endl;
		abort();
	}
}

void ShapeAndColorDetection::findPinkShapes()
{
	inRange(hsv, lowPink, highPink, mask);

	findContours(mask.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	labelPolygon(approx, contours, "ROZE", specificShape);
}

bool ShapeAndColorDetection::setShape(String shape)
{
	vector<string> shapes
	{ "driehoek", "rechthoek", "cirkel", "halve cirkel", "vierkant", "all" };
	for (uint16_t i = 0; i < shapes.size(); ++i)
	{
		if (shape == shapes.at(i))
		{
			specificShape = shape;
			return true;
		}
	}
	return false;
}

bool ShapeAndColorDetection::setColor(String color)
{
	//string setcolor = color;
	vector<string> colors
	{ "roze", "oranje", "groen", "geel", "all" };

	if (color.find("#"))
	{
		color = color.substr(0, color.find("#"));

	}

	for (uint16_t i = 0; i < colors.size(); ++i)
	{
		if (color == colors.at(i))
		{
			specificColor = color;
			return true;
		}
	}
	return false;
}

void ShapeAndColorDetection::logObject(const String& object,const string& surface,
		const string& xCord, const string& yCord)
{
	cout << "! Found " << object << " at cords: X:" << xCord << " Y:" << yCord
			<< " with surface area of " << surface << " in " << getClockTicks()
			<< " clock ticks !" << endl;
	objectHasBeenFound = true;
}

long ShapeAndColorDetection::getClockTicks()
{
	long ticks = clock() - clockTicks;
	clockTicks = clock();
	return ticks;

}

//void ShapeAndColorDetection::setSrc(string source)
//{
//	String path = samples::findFile(source);
//	src = imread(path, IMREAD_COLOR);
//}

void ShapeAndColorDetection::processBatchFile(string filename)
{
	batchEnabled = true;
	ifstream ifs
	{ filename };

	if (ifs.is_open())
	{
		string parameter;
		//string source;
		string color;
		string shape;

		while (!ifs.eof())
		{
			getline(ifs, parameter, '\n');

			if (!(parameter[0] == '#'))
			{
				stringstream sstream(parameter);
				//getline(sstream, source, ' ');
				getline(sstream, shape, ' ');

				if (shape == "halve")
				{
					string tmp;
					getline(sstream, tmp, ' ');
					shape = shape + " " + tmp;
				}

				getline(sstream, color, ' ');
				if (color.find("#"))
				{
					color = color.substr(0, color.find("#"));

				}
				cout << shape << " : " << color << endl;

				//setSrc(source);

				if (!setShape(shape))
				{
					cout << "unsupported shape: " << shape << endl;
							//<< "terminating program" << endl;
					skip = true;

				}

				if (!setColor(color))
				{
					cout << "unsupported color: " << color << endl;
						//	<< "terminating program" << endl;
					skip = true;
				}
				if(!skip)
				{
				detect();
				}
				skip = false;
			}

		}
		ifs.close();

	}
	exit(0);
}

void ShapeAndColorDetection::objectFound()
{
	if (!objectHasBeenFound)
	{
		cout
				<< "No object found of the specified shape and color in the given image. Elapsed time: "<< getClockTicks() <<" clock ticks."
				<< endl;
		objectHasBeenFound = false;
	}
	return;
}

// int ShapeAndColorDetection::getColorIndex(const string& color)
//{
//	vector<string> colors
//	{ "roze", "oranje", "groen", "geel", "all" };
//
//	for (uint64_t i = 0; i < colors.size(); ++i)
//	{
//		if (color == colors[i])
//		{
//			return i;
//		}
//	}
//	return 99;
//}
