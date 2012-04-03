//============================================================================
// Name        : aero-image-analyzer.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
//#include <opencv-2.3.1/opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;

Mat src; 

float ransacReprojThreshold=3.0;

int main( int argc, char** argv ) {
	// 1. Create a new console project. Read two input images.
	// Read two input images.
	Mat img_object = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_scene = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
	
	// 2. Detect keypoints in both images.
	//	FastFeatureDetector detector(30); // detecting keypoints (FastFeatureDetector)
	SurfFeatureDetector detector( 400 ); // detecting keypoints (Surf detector)
	vector<KeyPoint> keypoints_object, keypoints_scene;
	detector.detect(img_object, keypoints_object);
	detector.detect(img_scene, keypoints_scene);
	
	// 3. Compute descriptors for each of the keypoints.
	// computing descriptors
	SurfDescriptorExtractor extractor;
	Mat descriptors_object, descriptors_scene;
	extractor.compute(img_object, keypoints_object, descriptors_object);
	extractor.compute(img_scene, keypoints_scene, descriptors_scene);
	
	// 4. Matching descriptors
	//BruteForceMatcher<L2<float> > matcher;
	FlannBasedMatcher matcher;
	vector<DMatch> matches;
	matcher.match(descriptors_object, descriptors_scene, matches);
	
	//-- Quick calculation of max and min distances between keypoints
	double max_dist = 0; 
	double min_dist = 50;
	for( int i = 0; i < descriptors_object.rows; i++ )
	{ double dist = matches[i].distance;
		if( dist < min_dist ) min_dist = dist;
		if( dist > max_dist ) max_dist = dist;
	}
	printf("-- Max dist : %f \n", max_dist );
	printf("-- Min dist : %f \n", min_dist );
	
	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;
	for( int i = 0; i < descriptors_object.rows; i++ )
	{ if( matches[i].distance < 3*min_dist )
		{ good_matches.push_back( matches[i]); }
	}
	
	// 5. Visualize the results:
	namedWindow("matches", 1);
	Mat img_matches;
	//drawMatches(img_object, keypoints_object, img_scene, keypoints_scene, matches, img_matches);
//	drawMatches(img_object, keypoints_object, img_scene, keypoints_scene, good_matches, img_matches);
	drawMatches(img_object, keypoints_object, img_scene, keypoints_scene, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	imshow("matches", img_matches);
	
	// 6. Find the homography transformation between two sets of points:
	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for( int i = 0; i < good_matches.size(); i++ )
	{
		//-- Get the keypoints from the good matches
		obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
		scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
	}
	
	//Mat H = findHomography( obj, scene, CV_RANSAC );
//	Mat H = findHomography(obj, scene, CV_RANSAC, ransacReprojThreshold);
	
	
//	vector<Point2f> points1, points2;
	// fill the arrays with the points (Q: the points from matches?)
	//....
	
//	points1 = keypoints_object;
//	points2 = keypoints_scene;
//	Mat H = findHomography(Mat(keypoints_object), Mat(keypoints_scene), CV_RANSAC, ransacReprojThreshold);
	
//	// 7. Create a set of inlier matches and draw them. Use perspectiveTransform function to map points with homography:
//	Mat points1Projected; perspectiveTransform(Mat(points1), points1Projected, H);
//	
	// display the image (for debugging only, won't be used for final headless version of aero-image-analyzer)	
//	namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
//	imshow( "Display Image", img_object );
//	
//	waitKey(0);
	waitKey(0);
	
	
	return 0;
}
