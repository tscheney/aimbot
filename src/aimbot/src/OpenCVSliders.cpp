
#include "OpenCVSliders.h"

OpenCVSliders::OpenCVSliders(string title)
{
	LowH = 0;
	HighH = 179;

	LowS = 0;
	HighS = 255;

	LowV = 0;
	HighV = 255;

	namedWindow(title, CV_WINDOW_AUTOSIZE); //create a window called "Control"

	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);

}


