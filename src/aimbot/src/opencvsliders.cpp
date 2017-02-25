
#include "opencvsliders.h"


int OpenCVSliders::LowH = 0;
int OpenCVSliders::HighH = 179;
int OpenCVSliders::LowS = 0;
int OpenCVSliders::HighS = 255;
int OpenCVSliders::LowV = 0;
int OpenCVSliders::HighV = 255;


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
    printf("change pointers\n\r");
	name = inname;
	
    printf("create window\n\r");
	namedWindow(inname, CV_WINDOW_AUTOSIZE); //create a window called "Control"
    printf("create trackbars\n\r");
	createTrackbars(); // Create the trackbars for the window
}




OpenCVSliders::OpenCVSliders(string inname, int lh, int  hh, int ls, int hs, int lv, int hv) //: name(inname), LowH(lh), HighH(hh), LowS(ls), HighS(hs), LowV(lv), HighV(hv)
{
	name = inname;
/*
	lh = lh;
    hh = hh;
    ls = ls;
    hs = hs;
    lv = lv;
    hv = hv;

	LowH = &lh;
	HighH = &hh;

	LowS = &ls;
	HighS = &hs;

	LowV = &lv;
	HighV = &hv;*/

	namedWindow(inname, CV_WINDOW_AUTOSIZE); //create a window called "Control"

	createTrackbars(); // Create the trackbars for the window
}


void OpenCVSliders::createTrackbars()
{
	//createTrackbar("LowH", name.c_str(), &LowH, 179, &OpenCVSliders::callback, this); //Hue (0 - 179)
	createTrackbar("LowH", name.c_str(), &LowH, 179);
	createTrackbar("HighH", name.c_str(), &HighH, 179);

    createTrackbar("LowS", name.c_str(), &LowS, 255); //Saturation (0 - 255)
    createTrackbar("HighS", name.c_str(), &HighS, 255);

    createTrackbar("LowV", name.c_str(), &LowV, 255); //Value (0 - 255)
    createTrackbar("HighV", name.c_str(), &HighV, 255);

}

void OpenCVSliders::callback(int value, void* nothing)
{
    LowH = value;
}

void OpenCVSliders::exportScalar(Scalar scalarlh[2])
{
	Scalar scalelow = Scalar(LowH, LowS, LowV);
	printf("(%d, %d, %d)\n\r", LowH, LowS, LowV);
	Scalar scalehigh = Scalar(HighH, HighS, HighV);

	scalarlh[0] = scalelow;
	scalarlh[1] = scalehigh;
}





