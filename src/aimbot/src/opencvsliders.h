#ifndef OPENCVSLIDERS_H
#define OPENCVSLIDERS_H


#include <cv.h>
#include <highgui.h>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace std;
using namespace cv;

class OpenCVSliders
{
public:

    /*int lh;
    int  hh;
    int ls;
    int hs;
    int lv;
    int hv; */

    //Todo:: these can't be static, because they have to be different for each class
	static int LowH;
	static int HighH;

	static int LowS;
	static int HighS;

	static int LowV;
	static int HighV;

	string name;

	OpenCVSliders();

	OpenCVSliders(string inname);
	//OpenCVSliders(string inname);

	OpenCVSliders(string inname, int lh, int  hh, int ls, int hs, int lv, int hv);
	//OpenCVSliders(string inname, int lh, int  hh, int ls, int hs, int lv, int hv);

	void createTrackbars();

	static void callback(int value, void* nothing);

	void exportScalar(Scalar scalarlh[2]); 

};

#endif