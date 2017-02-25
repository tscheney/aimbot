
#include "opencvsliders.h"

OpenCVSliders::OpenCVSliders() //: name("blank"), LowH(0), HighH(179), LowS(0), HighS(255), LowV(0), HighV(255)
{
	/*
	name = inname;

	LowH = 0;
	HighH = 179;

	LowS = 0;
	HighS = 255;

	LowV = 0;
	HighV = 255;
	*/
	

	//namedWindow(inname, CV_WINDOW_AUTOSIZE); //create a window called "Control"

	//createTrackbars(); // Create the trackbars for the window
}

OpenCVSliders::OpenCVSliders(string inname) //: name(inname), LowH(0), HighH(179), LowS(0), HighS(255), LowV(0), HighV(255)
{
	name = inname;

	LowH = 0;
	HighH = 179;

	LowS = 0;
	HighS = 255;

	LowV = 0;
	HighV = 255;
	

	namedWindow(inname, CV_WINDOW_AUTOSIZE); //create a window called "Control"

	createTrackbars(); // Create the trackbars for the window
}


OpenCVSliders::OpenCVSliders(string inname, int lh, int  hh, int ls, int hs, int lv, int hv) //: name(inname), LowH(lh), HighH(hh), LowS(ls), HighS(hs), LowV(lv), HighV(hv)
{
	name = inname;

	LowH = lh;
	HighH = hh;

	LowS = ls;
	HighS = hs;

	LowV = lv;
	HighV = hv;

	namedWindow(inname, CV_WINDOW_AUTOSIZE); //create a window called "Control"

	createTrackbars(); // Create the trackbars for the window
}


void OpenCVSliders::createTrackbars()
{
	cvCreateTrackbar("LowH", "Control", &LowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &HighH, 179);

    cvCreateTrackbar("LowS", "Control", &LowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &HighS, 255);

    cvCreateTrackbar("LowV", "Control", &LowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &HighV, 255);
}

void OpenCVSliders::exportScalar(Scalar scalarlh[2])
{
	Scalar scalelow = Scalar(LowH, LowS, LowV);
	Scalar scalehigh = Scalar(HighH, HighS, HighV);

	scalarlh[0] = scalelow;
	scalarlh[1] = scalehigh;
}





