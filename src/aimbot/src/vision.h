#ifndef VISION_H
#define VISION_H

#include<QObject>
#include<QVector>
#ifndef Q_MOC_RUN
#include "ros/ros.h"
#include <image_transport/image_transport.h>
#endif
#include "geometry_msgs/Pose2D.h"
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class Vision : public QObject
{   Q_OBJECT
private:
	static string space;
private slots:
    void process(cv::Mat frame);
    void newHSV(QVector<int> hsv);
signals:
    void processedImage(cv::Mat frame);
public:
    Vision(QObject *parent = 0, string initName = "default");
	// constants
	static float FIELD_WIDTH;  // in meters
	static float FIELD_HEIGHT; 
	static float ROBOT_RADIUS;
	static float FIELD_WIDTH_PIXELS; // measured from threshold of goal to goal
	static float FIELD_HEIGHT_PIXELS; // measured from inside of wall to wall
	static float CAMERA_WIDTH;
	static float CAMERA_HEIGHT;
	static float FIELD_X_OFFSET;
    static float FIELD_Y_OFFSET;

    // Processing Params
    QVector<int> hsvParams;

    string name;
    ros::Publisher pub;
    ros::NodeHandle nh;

    // methods
    void initSliders();
    void initPublishers(string name);
    Mat thresholdImage(Mat& imgHSV, Scalar color[]);
    Mat detectShapes(Mat frame, Mat imgGray);
    vector<Moments> calcMoments(Mat imgGray);
	Point2d getCenterOfMass(Moments moment);
	static bool compareMomentAreas(Moments moment1, Moments moment2);
	Point2d imageToWorldCoordinates(Point2d point_i);
    geometry_msgs::Pose2D getRobotPose(vector<Moments> mm);
    geometry_msgs::Pose2D getBallPose(vector<Moments> mm);
	void imageCallback(const sensor_msgs::ImageConstPtr& msg);
    void publish(geometry_msgs::Pose2D& pos);
	void sendBallMessage(int x, int y);
	void mouseCallback(int event, int x, int y, int flags, void* userdata);
	bool ok();

};

#endif
