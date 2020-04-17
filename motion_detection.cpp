#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include <wiringPi.h>
#include <csignal> 
#include "blob.h"
using namespace std;
using namespace cv;

#ifdef WINDOWS
#endif





  // global flag used to exit from the main loop
  bool RUNNING = true;
  
  // Blink an LED

  void blink_led(int led, int time) {
     digitalWrite(led, HIGH);
     delay(time);
     digitalWrite(led, LOW);
     delay(time);
 }
 
 // Callback handler if CTRL-C signal is detected
 void my_handler(int s) {
    std::cout << "Detected CTRL-C signal no. " << s << '\n';
    RUNNING = false;
 }








// global variables ///////////////////////////////////////////////////////////////////////////////
const Scalar SCALAR_BLACK = Scalar(0.0, 0.0, 0.0);
const Scalar SCALAR_WHITE = Scalar(255.0, 255.0, 255.0);
const Scalar SCALAR_BLUE = Scalar(255.0, 0.0, 0.0);
const Scalar SCALAR_GREEN = Scalar(0.0, 200.0, 0.0);
const Scalar SCALAR_RED = Scalar(0.0, 0.0, 255.0);



///////////////////////////////////////////////////////////////////////////////////////////////////
Blob::Blob(std::vector<cv::Point> _contour) {

	contour = _contour;

	boundingRect = cv::boundingRect(contour);

	centerPosition.x = (boundingRect.x + boundingRect.x + boundingRect.width) / 2;
	centerPosition.y = (boundingRect.y + boundingRect.y + boundingRect.height) / 2;

	dblDiagonalSize = sqrt(pow(boundingRect.width, 2) + pow(boundingRect.height, 2));

	dblAspectRatio = (float)boundingRect.width / (float)boundingRect.height;

}

vector<long> time1;
int c = 0;

int main(void) {

	VideoCapture capVideo(0);

	Mat imgFrame1;
	Mat imgFrame2;

	capVideo.open(0);

	if (!capVideo.isOpened()) {                                                 // if unable to open video file
		cout << "\nerror reading video file" << endl << endl;      // show error message
#ifdef WINDOWS
		_getch();                    // it may be necessary to change or remove this line if not using Windows
#endif
		return(0);                                                              // and exit program
	}


						
				// Register a callback function to be called if the user presses CTRL-C
				std::signal(SIGINT, my_handler);
 
				// Initialize wiringPi and allow the use of BCM pin numbering
				wiringPiSetupGpio();
				
				

//	if (capVideo.get(CV_CAP_PROP_FRAME_COUNT) < 2) {
//		std::cout << "\nerror: video file must have at least two frames";
//#ifdef WINDOWS
//		_getch();
//#endif
//		return(0);
//	}


	capVideo >> imgFrame1;
	capVideo >> imgFrame2;

	char chCheckForEscKey = 0;

	while (capVideo.isOpened() && chCheckForEscKey != 27) {


		Mat imgFrame1Copy = imgFrame1.clone();
		Mat imgFrame2Copy = imgFrame2.clone();
		Mat imgDifference;
		Mat imgThresh;
		vector<Blob> blobs;



		cvtColor(imgFrame1Copy, imgFrame1Copy, CV_BGR2GRAY);
		cvtColor(imgFrame2Copy, imgFrame2Copy, CV_BGR2GRAY);

		GaussianBlur(imgFrame1Copy, imgFrame1Copy, Size(5, 5), 0);
		GaussianBlur(imgFrame2Copy, imgFrame2Copy, Size(5, 5), 0);



		absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

		threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY);

		imshow("imgThresh", imgThresh);

		float threshold = countNonZero(imgThresh);
		//cout << threshold << endl;
		time1.push_back(threshold);
		if (time1.size() == 30)
		{
			for (auto i = time1.begin(); i != time1.end(); ++i)
			{
				if (*i > 0)
					c++;
			}
			if (c > 15)
			{	cout << "on" << endl;
				

 
				//std::cout << "Controlling the GPIO pins with wiringPi\n";
 
				// Define the 3 pins we are going to use
				int red = 17, yellow = 22, green = 6;
 
				// Setup the pins
				pinMode(red, OUTPUT);
				//pinMode(yellow, OUTPUT);
				//pinMode(green, OUTPUT);
 
				//int time = 1;  
				
				digitalWrite(red, HIGH);
					//blink_led(red, time);
					//blink_led(yellow, time);
				//	blink_led(green, time);
			
				
			}
			else
			{	cout << "off" << endl;
               digitalWrite(17, LOW);
			
		
		}
		c = 0;
	    time1.clear();
		
	}
		
		

		Mat structuringElement3x3 = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat structuringElement5x5 = getStructuringElement(MORPH_RECT, Size(5, 5));
		Mat structuringElement7x7 = getStructuringElement(MORPH_RECT, Size(7, 7));
		Mat structuringElement9x9 = getStructuringElement(MORPH_RECT, Size(9, 9));

		dilate(imgThresh, imgThresh, structuringElement5x5);
		dilate(imgThresh, imgThresh, structuringElement5x5);
		erode(imgThresh, imgThresh, structuringElement5x5);

		Mat imgThreshCopy = imgThresh.clone();

		vector<vector<Point> > contours;

		findContours(imgThreshCopy, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


		Mat imgContours(imgThresh.size(), CV_8UC3, SCALAR_BLACK);

		drawContours(imgContours, contours, -1, SCALAR_WHITE, -1);

		//cv::imshow("imgContours", imgContours);

		vector<vector<Point> > convexHulls(contours.size());

		for (unsigned int i = 0; i < contours.size(); i++) {
			convexHull(contours[i], convexHulls[i]);
		}

		for (auto &convexHull : convexHulls) {
			Blob possibleBlob(convexHull);

			if (possibleBlob.boundingRect.area() > 100 &&
				possibleBlob.dblAspectRatio >= 0.2 &&
				possibleBlob.dblAspectRatio <= 1.2 &&
				possibleBlob.boundingRect.width > 15 &&
				possibleBlob.boundingRect.height > 20 &&
				possibleBlob.dblDiagonalSize > 30.0) {
				blobs.push_back(possibleBlob);
			}
		}

		cv::Mat imgConvexHulls(imgThresh.size(), CV_8UC3, SCALAR_BLACK);

		convexHulls.clear();

		for (auto &blob : blobs) {
			convexHulls.push_back(blob.contour);
		}

		cv::drawContours(imgConvexHulls, convexHulls, -1, SCALAR_WHITE, -1);

		//cv::imshow("imgConvexHulls", imgConvexHulls);




		imgFrame2Copy = imgFrame2.clone();



		for (auto &blob : blobs) {                                                  // for each blob
			cv::rectangle(imgFrame2Copy, blob.boundingRect, SCALAR_RED, 2);             // draw a red box around the blob
			cv::circle(imgFrame2Copy, blob.centerPosition, 3, SCALAR_GREEN, -1);        // draw a filled-in green circle at the center
		}

		cv::imshow("imgFrame2Copy", imgFrame2Copy);


		imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is
		//int key = waitKey(968);
		//if ((capVideo.get(CV_CAP_PROP_POS_FRAMES) + 1) < capVideo.get(CV_CAP_PROP_FRAME_COUNT)) {       // if there is at least one more frame
			capVideo>>imgFrame2;                            // read it
		//}
		//else {                                                  // else
		//	std::cout << "end of video\n";                      // show end of video message
		//	break;                                              // and jump out of while loop
		//}

		chCheckForEscKey = cv::waitKey(1000);      // get key press in case user pressed esc

	}

	if (chCheckForEscKey != 27) {  
		            // if the user did not press esc (i.e. we reached the end of the video)
		cv::waitKey(0); 
		                      // hold the windows open to allow the "end of video" message to show
	}
	
	if (chCheckForEscKey == 27)
	digitalWrite(17,LOW);   
	// note that if the user did press esc, we don't need to hold the windows open, we can simply let the program end which will close the windows

	return(0);

}
