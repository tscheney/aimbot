#ifndef VISION_H
#def VISION_H

#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"
#include "geometry_msgs/Vector3.h"
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>

#include <cmath>
#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;


class Vision
{
private:
	string amespace = "/aimbot_home/raw_vision/";
public:
	Vision();
	// constants
	const float FIELD_WIDTH = 3.53;  // in meters
	const float FIELD_HEIGHT = 2.39; 
	const float ROBOT_RADIUS = 0.10;
 	//const float GUI_NAME        "Soccer Overhead Camera"
	const float FIELD_WIDTH_PIXELS = 577.0; // measured from threshold of goal to goal
	const float FIELD_HEIGHT_PIXELS = 388.0; // measured from inside of wall to wall
	const float CAMERA_WIDTH = 640.0;
	const float CAMERA_HEIGHT = 480.0;

	// each objects stuff
	OpenCVSliders home1slide;
    OpenCVSliders home2slide;
    OpenCVSliders away1slide;
    OpenCVSliders away2slide;
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
	bool compareMomentAreas(Moments moment1, Moments moment2);
	Point2d imageToWorldCoordinates(Point2d point_i);
	void getRobotPose(Mat& imgHsv, Scalar color[], geometry_msgs::Pose2D& robotPose);
	void getBallPose(Mat& imgHsv, Scalar color[], geometry_msgs::Pose2D& ballPose);
	void processImage(Mat frame);
	void imageCallback(const sensor_msgs::ImageConstPtr& msg);
	void publish();
	void sendBallMessage(int x, int y);
	void mouseCallback(int event, int x, int y, int flags, void* userdata);

}

#endif