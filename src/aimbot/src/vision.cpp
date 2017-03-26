
#include "vision.h"
#include "moc_vision.cpp"

string Vision::space = "/aimbot_home/raw_vision/";
float Vision::FIELD_WIDTH = 3.53;  // in meters
float Vision::FIELD_HEIGHT = 2.39; 
float Vision::ROBOT_RADIUS = 0.10;
float Vision::FIELD_WIDTH_PIXELS = 610.0; // measured from threshold of goal to goal
float Vision::FIELD_HEIGHT_PIXELS = 426.0; // measured from inside of wall to wall
float Vision::FIELD_X_OFFSET = 111;
float Vision::FIELD_Y_OFFSET = 17;
float Vision::CAMERA_WIDTH = 864.0; //Todo check this it is likely that it is 848
float Vision::CAMERA_HEIGHT = 480.0;

Vision::Vision(QObject* parent, string initName) : QObject(parent)
{
    //connect(this, SIGNAL(started()), this, SLOT(process()));
    //moveToThread(this);
    name = initName;
    initPublishers(name);
    hsvParams.push_back(0);
    hsvParams.push_back(179);
    hsvParams.push_back(0);
    hsvParams.push_back(255);
    hsvParams.push_back(0);
    hsvParams.push_back(255);
}

// Processes the image
void Vision::process(cv::Mat frame)
{
    Mat imgHsv;
    geometry_msgs::Pose2D pos;
    cvtColor(frame, imgHsv, COLOR_BGR2HSV);
    Scalar scalarlh[2];
    Scalar scalelow = Scalar(hsvParams[0], hsvParams[2], hsvParams[4]);
    Scalar scalehigh = Scalar(hsvParams[1], hsvParams[3], hsvParams[5]);

    scalarlh[0] = scalelow;
    scalarlh[1] = scalehigh;

    Mat proc;

    // Hacky solution to get ball correct, probably should use inheritance here
    if(name == "ball")
    {
        proc = getBallPose(imgHsv, scalarlh, pos);
    }
    else
    {
        proc = getRobotPose(imgHsv, scalarlh, pos);
    }

    publish(pos);
    emit processedImage(proc);
}

// Update HSV params
void Vision::newHSV(QVector<int> hsv)
{
    for(int i = 0; i < hsv.size(); i++)
    {
        hsvParams[i] = hsv[i];
    }
}

// Initialize the publishers this class publishes
void Vision::initPublishers(string name)
{
    std::string key;
    if (nh.searchParam("team_side", key))
    {
        std::string val;
        nh.getParam(key, val);
        space = "/aimbot_" + val + "/raw_vision/";
    }    
    pub = nh.advertise<geometry_msgs::Pose2D>(space + name, 5);
}

// Thesholds the image to isolate the given colors
void Vision::thresholdImage(Mat& imgHSV, Mat& imgGray, Scalar color[])
{
    inRange(imgHSV, color[0], color[1], imgGray);

    erode(imgGray, imgGray, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)));
    dilate(imgGray, imgGray, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)));
}

// Gets the center point of a given moments
Point2d Vision::getCenterOfMass(Moments moment)
{
    double m10 = moment.m10;
    double m01 = moment.m01;
    double mass = moment.m00;
    double x = m10 / mass;
    double y = m01 / mass;
    return Point2d(x, y);
}

// Decides which moment has greater area
bool Vision::compareMomentAreas(Moments moment1, Moments moment2)
{
    double area1 = moment1.m00;
    double area2 = moment2.m00;
    return area1 < area2;
}

// Converts image pixels to world coordinates in meters
Point2d Vision::imageToWorldCoordinates(Point2d point_i)
{
    Point2d centerOfField(FIELD_X_OFFSET + FIELD_WIDTH_PIXELS/2, FIELD_Y_OFFSET + FIELD_HEIGHT_PIXELS/2);
    Point2d center_w = (point_i - centerOfField);

    // You have to split up the pixel to meter conversion
    // because it is a rect, not a square!
    center_w.x *= (FIELD_WIDTH/FIELD_WIDTH_PIXELS);
    center_w.y *= (FIELD_HEIGHT/FIELD_HEIGHT_PIXELS);

    // Reflect y
    center_w.y = -center_w.y;
    
    return center_w;
}

// Determines the position of a robot
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
    return imgThresholded;
}

// Determines the position of a ball
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

// Publishes the messages
void Vision::publish(geometry_msgs::Pose2D& pos)
{
    pub.publish(pos);
}

bool Vision::ok()
{
    return nh.ok();
}
