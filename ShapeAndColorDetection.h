/*
 * ShapeAndColorDetection.h
 *
 *  Created on: Sep 14, 2022
 *      Author: son
 */

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <map>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

//namespaces
using namespace cv;
using namespace std;

#ifndef SHAPEANDCOLORDETECTION_H_
#define SHAPEANDCOLORDETECTION_H_

class ShapeAndColorDetection
{
public:

	/**
	 * Constructor.
	 */
	ShapeAndColorDetection();

	/**
	 * Destructor
	 */
	virtual ~ShapeAndColorDetection();

	/**
	 * Parser function for ShapeAndColorDetection
	 * @param argc - defines number of arguments given. in this case 3
	 * @param **argv - defines the arguments given in command line
	 */
	void parse(int argc, char **argv);

	/**
	 * detection function for shapes and colors
	 */
	void detect();

private:

	/**
		 * function to convert i@src image to detecable image
		 *  @see hsv var
		 *  @see gray var
		 *  @see blackWhite var
		 *  @see mask var
		 *  @see dst var
		 */
	void convertimage();

	/**
		 * Function to find Colors and Shapes in the given source
		 * @see convertImage() function
		 *  @see findPinkShapes() function
		 *  @see findOrangeShapes() function
		 *  @see findGreenShapes() function
		 *  @see findYellowShapes() function
		 */
	void findColorsAndShapes();

	/**
	 * Function to find yellow Shapes
	 * @see labelPolygon Function
	 */
	void findYellowShapes();

	/**
	 * Function to find green Shapes
	 * @see labelPolygon Function
	 */
	void findGreenShapes();

	/**
	 * Function to find orange Shapes
	 * @see labelPolygon Function
	 */
	void findOrangeShapes();

	/**
	 * Function to find pink Shapes
	 * @see labelPolygon Function
	 */
	void findPinkShapes();

	/**
	 * Function to show specified shapes and color on the destination image
	 * @see videoEnabled var
	 * @see batchEnabled var
	 */
	void showImage();

	/**
	 * Function to find on live webcam
	 * @see convertImage() function
	 * @see findColorsAndShapes() function
	 * @see showImage() function
	 */
	void video();

	/**
	 * Function to set a label on a shape
	 * @param &im defines the image the label should be attached to
	 * @param label specifies the string on the label
	 * @param &countour specifies a vector of points of the shape to determine middle point
	 */
	void setLabel(cv::Mat &im, const std::string label,
			std::vector<cv::Point> &contour);

	/**
	 * Function to get the angle in degrees of three points
	 * @param pt1 specifies point 1
	 * @param pt2 specifies point 2
	 * @param pt0 specifies point 0
	 * @return a double of the angle in degrees
	 */
	double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);

	/**
	 * Function to try en determine if the given shape of lopygon
	 * @param &approxInput
	 * @param &contoursInput specifies a vector of point of the contour of the shape
	 * @param colortype specifies the color(HSV) of the mask used
	 * @param shape specifies the shape to be found as given in application arguments
	 * @see labelCircle if function is unable to determine if the shape is a polygon the function passes the values on to labelCircle function
	 */
	void labelPolygon(std::vector<cv::Point> &approxInput,
			vector<vector<cv::Point> > &contoursInput, String colortype,
			String shape);

	/**
	 * Function to to try and determine if the given shape is a circle
	 * @see @approxInput
	 * @param &contoursInput specifies a vector of point of the contour of the shape
	 * @param iterator specifies the intertion of the shape because drawContours() requires iterator as separate argument
	 * @param colortype specifies the color(HSV) of the mask used
	 * @param shape specifies the shape to be found as given in application arguments
	 */
	void labelCircle(std::vector<cv::Point> &approxInput,
			vector<vector<cv::Point> > &contoursInput, int iterator,
			String colortype, String shape);

	/**
	 * Setter function for shape
	 * @see specificShape var
	 * @return specifies if succesfull or not
	 */
	bool setShape(String shape);

	/**
	 * Setter function for color
	 * @see specificColor var
	 * @return specifies if succesfull or not
	 */
	bool setColor(String color);

	/**
	 * function to log found object on commandline. This function can be extended to log data to a file
	 * @param object specifies the shape of the object that has been found
	 * @param surface specifies the surface area of the shape
	 * @param xCord specifties the x coordinates of the shape
	 * @param ycords specifeis the y coordinates of the shape
	 *
	 */
	void logObject(String object, string surface, string xCord, string yCord);

	/**
	 * function to specifie amout of clock ticks sinds last call. first 0 set in constructor
	 * @return gives the amount of clock thick
	 */
	long getClockTicks();

	/**
	 * Setter function for the source image
	 * @param gives the path of the source image
	 * @see src var
	 */
	void setSrc(string source);

	/**
	 * Function to process a batch of images and arguments at ounce
	 * @param filename specifies the path of the file to be found
	 * @info only accepts .txt files
	 */
	void processBatchFile(string filename);

	/**
	 * Function to notify the user if objects have been found or not
	 */
	void objectFound();

	/**
	 * function to return the index in a vector of colors as to be used in the state machine
	 * @param color specifeis the color
	 * @see findColorsAndShapes() function
	 */
	int getColorIndex(string color);

	//mat variables

	/**
	 * specifies the source image
	 */
	Mat src;

	/**
	 * specifies the grayscale image
	 */
	Mat gray;

	/**
	 * specifies the black and white image
	 */
	Mat blackwhite;

	/**
	 * specifies the image in hsv color ranges
	 */
	Mat hsv;

	/**
	 * specifies the image mask
	 */
	Mat mask;

	/**
	 * specifies the image destination
	 */
	Mat dst;

	//color variables

	/**
	 * specifies the lowest range of green to be detected
	 */
	Scalar lowGreen;

	/**
	 * specifies the highest range of green to be detected
	 */
	Scalar highGreen;

	/**
	 * specifies the lowest range of pink to be detected
	 */
	Scalar lowPink;

	/**
	 * specifies the highest range of pink te be detected
	 */
	Scalar highPink;

	/**
	 * specifies the lowest range of orange to be detected
	 */
	Scalar lowOrange;

	/**
	 * specifies the highest range of orange to be detected
	 */
	Scalar highOrange;

	/**
	 * specifies the lowest range of yellow to be detected
	 */
	Scalar lowYellow;

	/**
	 * specifies the highest range to be detected
	 */
	Scalar highYellow;

	/**
	 * specifies the color as given by the parser
	 */
	String specificColor;

	/**
	 * specifies the shape as given by the parses
	 */
	String specificShape;

	/**
	 * specifies the video camera source
	 */
	VideoCapture vid;

	//flags

	/**
	 * flag to check video camera is to be used
	 */
	bool videoEnabled;

	/**
	 * flag to check if batch processing is to be used
	 */
	bool batchEnabled;

	/**
	 * specifies the last amount of clock ticks
	 */
	long clockTicks;

	/**
	 * flag ot check if object has been found or not
	 */
	bool objectHasBeenFound;
	//bool readyForNextFrame;

	//vectors

	/**
	 * vector of vector of point of the found shapes
	 */
	vector<vector<cv::Point> > contours;

	/**
	 * vector of approximated points of the shape
	 */
	vector<cv::Point> approx;

};

#endif /* SHAPEANDCOLORDETECTION_H_ */
