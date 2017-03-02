
#include "opencvsliders.h"


int OpenCVSliders::lhball = 0;
int OpenCVSliders::hhball = 179;
int OpenCVSliders::lsball = 0;
int OpenCVSliders::hsball = 255;
int OpenCVSliders::lvball = 0;
int OpenCVSliders::hvball = 255;

int OpenCVSliders::lhhome1 = 0;
int OpenCVSliders::hhhome1 = 19;
int OpenCVSliders::lshome1 = 139;
int OpenCVSliders::hshome1 = 186;
int OpenCVSliders::lvhome1 = 219;
int OpenCVSliders::hvhome1 = 231;


OpenCVSliders::OpenCVSliders()
{
}


OpenCVSliders::OpenCVSliders(string inname)
{
    name = inname;

	namedWindow(name, CV_WINDOW_AUTOSIZE); //create a window called "Control"
	createTrackbars(); // Create the trackbars for the window
}

void OpenCVSliders::createTrackbars()
{
    if(name=="home1")
    {
        createTrackbar("LowH", name.c_str(), &lhhome1, 179);
        createTrackbar("HighH", name.c_str(), &hhhome1, 179);

        createTrackbar("LowS", name.c_str(), &lshome1, 255); //Saturation (0 - 255)
        createTrackbar("HighS", name.c_str(), &hshome1, 255);

        createTrackbar("LowV", name.c_str(), &lvhome1, 255); //Value (0 - 255)
        createTrackbar("HighV", name.c_str(), &hvhome1, 255);
    }
    else if(name=="ball")
    {
        //createTrackbar("LowH", name.c_str(), &LowH, 179, &OpenCVSliders::callback, this); //Hue (0 - 179)
        createTrackbar("LowHb", name.c_str(), &lhball, 179);
        createTrackbar("HighHb", name.c_str(), &hhball, 179);

        createTrackbar("LowSb", name.c_str(), &lsball, 255); //Saturation (0 - 255)
        createTrackbar("HighSb", name.c_str(), &hsball, 255);

        createTrackbar("LowVb", name.c_str(), &lvball, 255); //Value (0 - 255)
        createTrackbar("HighVb", name.c_str(), &hvball, 255);
    }
}

void OpenCVSliders::exportScalar(Scalar scalarlh[2])
{
    Scalar scalelow = Scalar(0, 0, 0);
    Scalar scalehigh = Scalar(179, 255, 255);
    if(name=="home1")
    {
        scalelow = Scalar(lhhome1, lshome1, lvhome1);
        scalehigh = Scalar(hhhome1, hshome1, hvhome1);

    }
    else if(name=="ball")
    {
        scalelow = Scalar(lhball, lsball, lvball);
        scalehigh = Scalar(hhball, hsball, hvball);
    }


	scalarlh[0] = scalelow;
	scalarlh[1] = scalehigh;
}





