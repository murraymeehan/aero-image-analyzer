/**
 * @file HoughLines_Demo.cpp
 * @brief Demo code for Hough Transform
 * @author OpenCV team
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat src;

void findSquares(Mat &src, Mat src_gray, vector<Vec3f> &squares) 
{
	Mat edges; 
	int min_threshold = 10;

	//////////////
	// Apply Canny edge detector
	Canny( src_gray, edges, 50, 200, 3 );

	/// Create Trackbars for Thresholds
	char thresh_label[50];
	sprintf( thresh_label, "Thres: %d + input", min_threshold );

	/// 2. Use Probabilistic Hough Transform
	vector<Vec4i> p_lines;
	Mat probabilistic_hough;
	cvtColor( edges, probabilistic_hough, CV_GRAY2BGR );
	HoughLinesP( edges, p_lines, 1, CV_PI/180, min_threshold, 30, 10 );

	/// Show the result
	for( size_t i = 0; i < p_lines.size(); i++ )
	{
		Vec4i l = p_lines[i];
		//line( probabilistic_hough, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
		line( src, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
		std::cout << "lines detected @ " << Point(l[0], l[1])<<","<< Point(l[2], l[3]) << ".\n";
	}
	
	// TODO: identify pairs of same-length same-orientation lines
	// TODO: identify squares as 2 sets of these pairs, report location
}

/**
 * @function main
 */
int main( int argc, char** argv )
{
   //	Mat src = imread( argv[1], 1 );
	Mat src = imread( "shapes.jpg",1);
	if( !src.data )
		{ return -1; }

	Mat channelRed (src.rows, src.cols, CV_8UC1);
	Mat channelGreen (src.rows, src.cols, CV_8UC1);
	Mat channelBlue (src.rows, src.cols, CV_8UC1);
	Mat outImg[] = {channelRed, channelGreen, channelBlue};
	int fromTo[] = { 0,2, 1,1, 2,0};
	mixChannels( &src, 1, outImg, 3, fromTo, 3);
	
   	vector<Vec3f> squaresR, squaresG, squaresB;
	findSquares(src, channelRed, squaresR);
	findSquares(src, channelGreen, squaresG);
	findSquares(src, channelBlue, squaresB);
   
   
   return 0;
}

