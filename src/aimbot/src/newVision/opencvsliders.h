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

    //Todo:: these can't be static, because they have to be different for each class
	static int lhball;
	static int hhball;
	static int lsball;
	static int hsball;
	static int lvball;
	static int hvball;

	static int lhhome1;
	static int hhhome1;
	static int lshome1;
	static int hshome1;
	static int lvhome1;
	static int hvhome1;

	string name;

	OpenCVSliders();

	OpenCVSliders(string name);

	void createTrackbars();

	void exportScalar(Scalar scalarlh[2]);

};

#endif