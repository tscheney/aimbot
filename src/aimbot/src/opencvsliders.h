#ifndef OPENCVSLIDERS_H
#def OPENCVSLIDERS_H


#include <cv.h>
#include <highgui.h>
#include <stirng>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace cv;

class OpenCVSliders
{
public:
	int LowH;
	int HighH;

	int LowS;
	int HighS;

	int LowV;
	int HighV;

	string name;

	OpenCVSliders(string inname);

	OpenCVSliders(string inname, lh, hh, ls, hs, lv, hv);

	void createTrackbars();

	void exportScalar(Scalar scalarlh[2]); 

}

#endif