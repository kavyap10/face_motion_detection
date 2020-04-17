/*
#include <stdio.h>  // For printf
#include <iostream>
#include <opencv/cv.hpp>  
#include <opencv/cxcore.hpp>  
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
int main()
{


	

//||||||||||||||||||||||||||------------BRIEF INTERMISSION---------------||||||||||||||||||||||| 








// Capture the Image from the webcam
VideoCapture cap(0);

// Get the frame
Mat save_img;
cap >> save_img;

if (save_img.empty())
{
	std::cerr << "Something is wrong with the webcam, could not get frame." << std::endl;
}
// Save the frame into a file
imwrite("reference.jpg", save_img); // A JPG FILE IS BEING SAVED

String window_original = "original";
//cv::imshow(window_original, save_img);







	//VideoCapture cap(0);
	//cap.open(0);

cap >> save_img;
imwrite("reference.jpg", save_img);

cap >> save_img;
imwrite("reference.jpg", save_img);

	for (;;)
	{

	// Image from camera to Mat
	Mat img;
	cap >> img;
	imwrite("frame.jpg", img); // A JPG FILE IS BEING SAVED
	// obtain input image from source
	cap.retrieve(img, CV_CAP_OPENNI_BGR_IMAGE);

	// Just resize input image if you want
	//resize(img, img, Size(1000, 640));




	//imshow("wooohooo", img);
	//int key2 = waitKey(9000);

	String filename1 = "/home/pi/Desktop/Motion detection/reference.jpg";
	String filename2 = "/home/pi/Desktop/Motion detection/frame.jpg";
	//cv::String window_original = "original";
	String window_gray = "gray"; 
	String window_frame = "frameGray";
	String window_result = "results";
	Mat reference = imread(filename1);
	Mat frames = imread(filename2);
	//cv::Mat grayMat;
	//cv::Mat frameGrayMat;
	Mat imgDifference;
	Mat imgThresh;
	

	cvtColor(reference, reference, COLOR_BGR2GRAY);
	//cv::namedWindow(window_gray, cv::WINDOW_AUTOSIZE);
	//cv::imshow(window_gray, grayMat);
	cvtColor(frames, frames, COLOR_BGR2GRAY);
	//cv::namedWindow(window_frame, cv::WINDOW_AUTOSIZE);
	//cv::imshow(window_frame, frameGrayMat);


	GaussianBlur(reference, reference, Size(5, 5), 0);
	GaussianBlur(frames, frames, Size(5, 5), 0);


	absdiff(frames,reference, imgDifference);
	threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY);
	namedWindow(window_result, WINDOW_AUTOSIZE);
	imshow(window_result, imgThresh);
	
	float threshold = countNonZero(imgThresh);
	cout << threshold<<endl;
	int key2 = waitKey(100);

     }





	return 0;
}
*/



#include <stdio.h>  // For printf
#include <iostream>
#include <opencv/cv.hpp>  
#include <opencv/cxcore.hpp>  
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
int main()
{



/*||||||||||||||||||||||||||------------BRIEF INTERMISSION---------------||||||||||||||||||||||| */








// Capture the Image from the webcam
cv::VideoCapture cap(0);

// Get the frame
cv::Mat save_img;
cap >> save_img;

if (save_img.empty())
{
	std::cerr << "Something is wrong with the webcam, could not get frame." << std::endl;
}
// Save the frame into a file
imwrite("reference.jpg", save_img); // A JPG FILE IS BEING SAVED

cv::String window_original = "original";
//cv::imshow(window_original, save_img);







	//VideoCapture cap(0);
	//cap.open(0);
cap >> save_img;
imwrite("reference.jpg", save_img);

cap >> save_img;
imwrite("reference.jpg", save_img);
	Mat img;


	for (;;)
	{

	// Image from camera to Mat

	cap >> img;
	imwrite("frame.jpg", img); // A JPG FILE IS BEING SAVED
	// obtain input image from source
	cap.retrieve(img, CV_CAP_OPENNI_BGR_IMAGE);

	// Just resize input image if you want
	resize(img, img, Size(1000, 640));




	//imshow("wooohooo", img);
	//int key2 = waitKey(9000);

	String filename1 = "/home/pi/Desktop/Motion detection/reference.jpg";
	String filename2 = "/home/pi/Desktop/Motion detection/frame.jpg";
	//cv::String window_original = "original";
	cv::String window_gray = "gray"; 
	cv::String window_frame = "frameGray";
	cv::String window_result = "results";
	cv::Mat reference = cv::imread(filename1);
	cv::Mat frames = cv::imread(filename2);
	//cv::Mat grayMat;
	//cv::Mat frameGrayMat;
	cv::Mat imgDifference;
	cv::Mat imgThresh;
	

	cvtColor(reference, reference, cv::COLOR_BGR2GRAY);
	//cv::namedWindow(window_gray, cv::WINDOW_AUTOSIZE);
	//cv::imshow(window_gray, grayMat);
	cvtColor(frames, frames, cv::COLOR_BGR2GRAY);
	//cv::namedWindow(window_frame, cv::WINDOW_AUTOSIZE);
	//cv::imshow(window_frame, frameGrayMat);


	cv::GaussianBlur(reference, reference, cv::Size(5, 5), 0);
	cv::GaussianBlur(frames, frames, cv::Size(5, 5), 0);


	absdiff(reference,frames, imgDifference);
	cv::threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY);
	cv::namedWindow(window_result, cv::WINDOW_AUTOSIZE);
	cv::imshow(window_result, imgThresh);
	double size = 640000;
	double threshold = countNonZero(imgThresh);
	double percent = (threshold / size) * 100;
	//cout << percent<<endl;
	if (percent < 2)
		cout << "off" << endl;
	else
		cout << "on" << endl;
	int key2 = waitKey(1000);

     }





	return 0;
}
