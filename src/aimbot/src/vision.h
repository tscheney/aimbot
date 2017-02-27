#ifndef VISION_H
#define VISION_H

#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"
#include "geometry_msgs/Vector3.h"
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include "opencvsliders.h"

#include <cmath>
#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;


class Vision
{
private:
	static string space;
public:
	Vision();
	// constants
	static float FIELD_WIDTH;  // in meters
	static float FIELD_HEIGHT; 
	static float ROBOT_RADIUS;
 	static string GUI_NAME; 
	static float FIELD_WIDTH_PIXELS; // measured from threshold of goal to goal
	static float FIELD_HEIGHT_PIXELS; // measured from inside of wall to wall
	static float CAMERA_WIDTH;
	static float CAMERA_HEIGHT;
	static float FIELD_X_OFFSET;
	static float FIELD_Y_OFFSET;

	// each objects stuff
	OpenCVSliders home1slide;
    //static OpenCVSliders home2slide;
    //static OpenCVSliders away1slide;
    //static OpenCVSliders away2slide;
    OpenCVSliders ballslide;
	geometry_msgs::Pose2D poseHome1;
	geometry_msgs::Pose2D poseHome2;
	geometry_msgs::Pose2D poseAway1;
	geometry_msgs::Pose2D poseAway2;
	geometry_msgs::Pose2D poseBall;
	ros::Publisher home1_pub;
	ros::Publisher home2_pub;
	ros::Publisher away1_pub;
	ros::Publisher away2_pub;
	ros::Publisher ball_pub;
    ros::NodeHandle nh;

    // methods
    void initSliders();
    void initPublishers();
    void initSubscribers();
	void thresholdImage(Mat& imgHSV, Mat& imgGray, Scalar color[]);
	Point2d getCenterOfMass(Moments moment);
	static bool compareMomentAreas(Moments moment1, Moments moment2);
	Point2d imageToWorldCoordinates(Point2d point_i);
	Mat getRobotPose(Mat& imgHsv, Scalar color[], geometry_msgs::Pose2D& robotPose);
	Mat getBallPose(Mat& imgHsv, Scalar color[], geometry_msgs::Pose2D& ballPose);
	void processImage(Mat frame);
	void imageCallback(const sensor_msgs::ImageConstPtr& msg);
	void publish();
	void sendBallMessage(int x, int y);
	void mouseCallback(int event, int x, int y, int flags, void* userdata);
	bool ok();

};

#endif