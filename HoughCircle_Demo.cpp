/**
 * @file HoughCircle_Demo.cpp
 * @brief Demo code for Hough Transform
 * @author OpenCV team
 * @author Murray Meehan (added multi-channel scanning for my openCV tutorial)
 */

// uncomment this to turn on highgui-based display (for debugging / studying)
//#define DISPLAY_ENABLED

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;

void findSquare(Mat &src, Mat src_gray, vector<Vec3f> &circles) 
{
	/// Reduce the noise so we avoid false circle detection
	GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

	/// Apply the Hough Transform to find the circles
	int method=CV_HOUGH_GRADIENT;
	double dp=1;
	double minDist=src_gray.rows/8; 
	double param1=200;
	double param2=100; 
	int minRadius=3;//3; 
	int maxRadius=500;//100;
	HoughCircles( src_gray, circles, method, dp, minDist, param1, param2, minRadius, maxRadius );

	/// Draw the circles detected
	for( size_t i = 0; i < circles.size(); i++ )
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);

		#ifdef DISPLAY_ENABLED
			// circle center
			circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
			// circle outline
			circles( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
		#endif

		std::cout << "circle detected @ " << center << ", radius: " << radius << ".\n";
	}
}

/**
 * @function findCircles
 * 		TODO: only check for circles which are blue, red, or yellow.
 * 		TODO: only check for circles in size range from 2m*2m to 5m*5m 
 * 						(pixel equivalent depends on height (400m?), FOV (90deg), resolution (720p)
 * 						for conversion, see swath tables in dropbox for our GoPro Hero camera
 */
 void findCircles(Mat &src, Mat src_gray, vector<Vec3f> &circles) 
{
	/// Reduce the noise so we avoid false circle detection
	GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

	/// Apply the Hough Transform to find the circles
	int method=CV_HOUGH_GRADIENT;
	double dp=1;
	double minDist=src_gray.rows/8; 
	double param1=200;
	double param2=100; 
	int minRadius=3;//3; 
	int maxRadius=500;//100;
	HoughCircles( src_gray, circles, method, dp, minDist, param1, param2, minRadius, maxRadius );

	/// Draw the circles detected
	for( size_t i = 0; i < circles.size(); i++ )
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);

		#ifdef DISPLAY_ENABLED
			// circle center
			circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
			// circle outline
			circles( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
		#endif

		std::cout << "circle detected @ " << center << ", radius: " << radius << ".\n";
	}
}

/**
 * @function main
 */
 int main(int argc, char** argv)
{
	Mat src = imread( argv[1], 1 );

	if( !src.data )
		{ return -1; }

	Mat channelRed (src.rows, src.cols, CV_8UC1);
	Mat channelGreen (src.rows, src.cols, CV_8UC1);
	Mat channelBlue (src.rows, src.cols, CV_8UC1);
	Mat outImg[] = {channelRed, channelGreen, channelBlue};
	int fromTo[] = { 0,2, 1,1, 2,0};
	mixChannels( &src, 1, outImg, 3, fromTo, 3);

	/*
	namedWindow( "channelRed", CV_WINDOW_AUTOSIZE );
	imshow( "channelRed", channelRed );
	namedWindow( "channelGreen", CV_WINDOW_AUTOSIZE );
	imshow( "channelGreen", channelGreen );
	namedWindow( "channelBlue", CV_WINDOW_AUTOSIZE );
	imshow( "channelBlue", channelBlue );
	*/

	vector<Vec3f> circlesR, circlesG, circlesB;
	findCircles(src, channelRed, circlesR);
	findCircles(src, channelGreen, circlesG);
	findCircles(src, channelBlue, circlesB);

	#ifdef DISPLAY_ENABLED
	/// Show your results 
	namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
	imshow( "Hough Circle Transform Demo", src );
	waitKey(0);
	#endif

	return 0;
}
