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
#include <opencv2/ximgproc.hpp>
#include "global.h"

using namespace std;
using namespace cv;

class Vision : public QObject
{   Q_OBJECT
private:
    string name;
    bool isHome = true;

    Mat unFiltFrame;
    Mat preFiltFrame;

    // Processing Params
    ColorData colorData;
    //ShapeData shapeData;
    bool isUseBackSub = true;
    bool isUseColor = true;
    bool isUseEdgeDetect = true;
    bool isUseShape = true;

    ros::Publisher pub;
    ros::NodeHandle nh;

public slots:
    void process(cv::Mat frame);
    void newUnfilteredFrame(cv::Mat frame);
    void newPrefiltFrame(cv::Mat frame);
    void newColorData(ColorData newColorData);
    void useBackSub(bool value);
    void useColor(bool value);
    void useEdgeDetect(bool value);
    void useShape(bool value);
    void initPublishers();
    void newTeamSide(bool inIsHome);
signals:
    void processedImage(cv::Mat frame);
public:
    Vision(QObject *parent = 0, string initName = "default", bool inIsHome = true);
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

    geometry_msgs::Pose2D prevPos;

    // methods
    void initSliders();
    Mat applyMask(Mat frame, Mat mask);
    Mat thresholdImage(Mat& imgHSV, Scalar color[]);
    virtual Mat applyBlur(Mat frame) = 0;
    Mat applyBlurBase(Mat frame, int blurSize);
    virtual Mat detectShapes(Mat frame) = 0;
    Mat detectShapesBase(Mat frame, int blurSize, int edgeThresh1, int edgeThresh2, double polyError);
    virtual Mat detectShapeEdges(Mat frame) = 0;
    Mat applyDilateBase(Mat frame, int iterations);
    virtual Mat applyDilate(Mat frame) = 0;
    Mat detectShapeEdgesBase(Mat frame, int edgeThresh1, int edgeThresh2);
    virtual bool isCorrectSize(vector<Point> shape) = 0;
    virtual bool isCorrectShape(vector<Point> shape) = 0;
    Mat getShapeMask(Mat frame, vector<vector<Point>> sizeResults, vector<vector<Point>> shapeResults);
    virtual bool isShapeLargeEnough(vector<vector<Point>> shapeResults) = 0;
    Mat detectColors(Mat frame);
    Mat backgroundSubtraction(Mat frame);
    vector<Moments> calcMoments(Mat imgGray);
	Point2d getCenterOfMass(Moments moment);
	static bool compareMomentAreas(Moments moment1, Moments moment2);
	Point2d imageToWorldCoordinates(Point2d point_i);
    virtual geometry_msgs::Pose2D getPos(vector<Moments> mm) = 0;
	void imageCallback(const sensor_msgs::ImageConstPtr& msg);
    void publish(geometry_msgs::Pose2D& pos);
	void sendBallMessage(int x, int y);
	void mouseCallback(int event, int x, int y, int flags, void* userdata);
	bool ok();

};

#endif
