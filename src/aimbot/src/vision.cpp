
#include "vision.h"

string Vision::space = "/aimbot_home/raw_vision/";
string Vision::GUI_NAME = "home1";
float Vision::FIELD_WIDTH = 3.53;  // in meters
float Vision::FIELD_HEIGHT = 2.39; 
float Vision::ROBOT_RADIUS = 0.10;
float Vision::FIELD_WIDTH_PIXELS = 577.0; // measured from threshold of goal to goal
float Vision::FIELD_HEIGHT_PIXELS = 388.0; // measured from inside of wall to wall
float Vision::CAMERA_WIDTH = 640.0;
float Vision::CAMERA_HEIGHT = 480.0;

Vision::Vision()
{
    initSliders();
    //initSubscribers();
    initPublishers();
}

// Create OpenCV Windows and sliders
void Vision::initSliders()
{
    printf("begin initSliders\n\r");
    home1slide = OpenCVSliders("home1");
    //home2slide = OpenCVSliders("home2");
    //away1slide = OpenCVSliders("away1");
    //away2slide = OpenCVSliders("away2");
    ballslide = OpenCVSliders("ball");
    printf("end initSliders\n\r");
}


void Vision::initPublishers()
{
    printf("begin initPublishers\n\r");
    std::string key;
    if (nh.searchParam("team_side", key))
    {
        std::string val;
        nh.getParam(key, val);
        space = "/aimbot_" + val + "/raw_vision/";
    }
    
    home1_pub = nh.advertise<geometry_msgs::Pose2D>(space + "home1", 5);
    home2_pub = nh.advertise<geometry_msgs::Pose2D>(space + "home2", 5);
    away1_pub = nh.advertise<geometry_msgs::Pose2D>(space + "away1", 5);
    away2_pub = nh.advertise<geometry_msgs::Pose2D>(space + "away2", 5);
    ball_pub = nh.advertise<geometry_msgs::Pose2D>(space + "ball", 5);
    printf("end initPublishers\n\r");
}

void Vision::initSubscribers()
{
    printf("begin initSubscribers\n\r");
        // Subscribe to camera
    image_transport::ImageTransport it(nh);
    image_transport::Subscriber image_sub = it.subscribe("/usb_cam_away/image_raw", 1, &Vision::imageCallback, this);
    printf("end initSubscribers\n\r");
}



void Vision::thresholdImage(Mat& imgHSV, Mat& imgGray, Scalar color[])
{
    inRange(imgHSV, color[0], color[1], imgGray);

    erode(imgGray, imgGray, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)));
    dilate(imgGray, imgGray, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)));

    //imshow(GUI_NAME, imgGray);
}

Point2d Vision::getCenterOfMass(Moments moment)
{
    double m10 = moment.m10;
    double m01 = moment.m01;
    double mass = moment.m00;
    double x = m10 / mass;
    double y = m01 / mass;
    return Point2d(x, y);
}

bool Vision::compareMomentAreas(Moments moment1, Moments moment2)
{
    double area1 = moment1.m00;
    double area2 = moment2.m00;
    return area1 < area2;
}

Point2d Vision::imageToWorldCoordinates(Point2d point_i)
{
    Point2d centerOfField(CAMERA_WIDTH/2, CAMERA_HEIGHT/2);
    Point2d center_w = (point_i - centerOfField);

    // You have to split up the pixel to meter conversion
    // because it is a rect, not a square!
    center_w.x *= (FIELD_WIDTH/FIELD_WIDTH_PIXELS);
    center_w.y *= (FIELD_HEIGHT/FIELD_HEIGHT_PIXELS);

    // Reflect y
    center_w.y = -center_w.y;
    
    return center_w;
}

Mat Vision::getRobotPose(Mat& imgHsv, Scalar color[], geometry_msgs::Pose2D& robotPose)
{
    Mat imgGray;
    thresholdImage(imgHsv, imgGray, color);
    Mat imgThresholded = imgGray.clone();

    vector< vector<Point> > contours;
    vector<Moments> mm;
    vector<Vec4i> hierarchy;
    findContours(imgGray, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    if (hierarchy.size() != 2)
        return imgThresholded;



    for(int i = 0; i < hierarchy.size(); i++)
        mm.push_back(moments((Mat)contours[i]));

    std::sort(mm.begin(), mm.end(), compareMomentAreas);
    Moments mmLarge = mm[mm.size() - 1];
    Moments mmSmall = mm[mm.size() - 2];

    Point2d centerLarge = imageToWorldCoordinates(getCenterOfMass(mmLarge));
    Point2d centerSmall = imageToWorldCoordinates(getCenterOfMass(mmSmall));

    Point2d robotCenter = (centerLarge + centerSmall) * (1.0 / 2);
    Point2d diff = centerSmall - centerLarge;
    double angle = atan2(diff.y, diff.x);

    //convert angle to degrees
    angle = angle *180/M_PI;
    robotPose.x = robotCenter.x;
    robotPose.y = robotCenter.y;
    robotPose.theta = angle;
    return imgThresholded ;
}

Mat Vision::getBallPose(Mat& imgHsv, Scalar color[], geometry_msgs::Pose2D& ballPose)
{
    Mat imgGray;
    thresholdImage(imgHsv, imgGray, color);
    Mat imgThresholded = imgGray.clone();

    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(imgGray, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    if (hierarchy.size() != 1)
        return imgThresholded;

    Moments mm = moments((Mat)contours[0]);
    Point2d ballCenter = imageToWorldCoordinates(getCenterOfMass(mm));

    ballPose.x = ballCenter.x;
    ballPose.y = ballCenter.y;
    ballPose.theta = 0;
    return imgThresholded;
}

void Vision::processImage(Mat frame)
{
    Mat imgHsv;
    cvtColor(frame, imgHsv, COLOR_BGR2HSV);
    Scalar scalelh[2];

    // home 1 vision
    home1slide.exportScalar(scalelh);
    Mat home1 = getRobotPose(imgHsv, scalelh, poseHome1);
    imshow("home1", home1);


    home2slide.exportScalar(scalelh);
    getRobotPose(imgHsv, scalelh,  poseHome2);

    away1slide.exportScalar(scalelh);
    getRobotPose(imgHsv, scalelh,    poseAway1);

    away2slide.exportScalar(scalelh);
    getRobotPose(imgHsv, scalelh, poseAway2);

    // ball vision
    ballslide.exportScalar(scalelh);
    Mat ball = getBallPose(imgHsv,  scalelh, poseBall);
    imshow("ball", ball);

    publish();    
}

void Vision::publish()
{
    home1_pub.publish(poseHome1);
    home2_pub.publish(poseHome2);
    away1_pub.publish(poseAway1);
    away2_pub.publish(poseAway2);
    ball_pub.publish(poseBall);
}


void Vision::imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
        Mat frame = cv_bridge::toCvShare(msg, "bgr8")->image;
        processImage(frame);
        //imshow(GUI_NAME, frame);
        waitKey(30);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

bool Vision::ok()
{
    return nh.ok();
}

/*
void Vision::sendBallMessage(int x, int y)
{
    // Expects x, y in pixels

    // Convert pixels to meters for sending  simulated ball position from mouse clicks
    float x_meters, y_meters;

    // shift data by half the pixels so (0, 0) is in center
    x_meters = x - (CAMERA_WIDTH/2.0);   
    y_meters = y - (CAMERA_HEIGHT/2.0);

    // Multiply by aspect-ratio scaling factor
    x_meters = x_meters * (FIELD_WIDTH/FIELD_WIDTH_PIXELS);
    y_meters = y_meters * (FIELD_HEIGHT/FIELD_HEIGHT_PIXELS);

    // mirror y over y-axis
    y_meters = -1*y_meters;

    // cout << "x: " << x_meters << ", y: " << y_meters << endl;

    geometry_msgs::Vector3 msg;
    msg.x = x_meters;
    msg.y = y_meters;
    msg.z = 0;
    ball_position_pub.publish(msg);
} */

/*
void Vision::mouseCallback(int event, int x, int y, int flags, void* userdata) {
    static bool mouse_left_down = false;

    if (event == EVENT_LBUTTONDOWN) {
        mouse_left_down = true;
        Point2d point_meters = imageToWorldCoordinates(Point2d(x, y));
        char buffer[50];
        sprintf(buffer, "Location: (%.3f m, %.3f m)", point_meters.x, point_meters.y);
        displayStatusBar(GUI_NAME, buffer, 10000);

    //} else if (event == EVENT_MOUSEMOVE) {
       // if (mouse_left_down) sendBallMessage(x, y);

    } else if (event == EVENT_LBUTTONUP) {
        sendBallMessage(x, y);
        mouse_left_down = false;
    }
    
}
*/