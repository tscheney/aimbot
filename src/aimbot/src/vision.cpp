
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

// Update HSV params
void Vision::newHSV(QVector<int> hsv)
{
    for(int i = 0; i < hsv.size(); i++)
    {
        hsvParams[i] = hsv[i];
    }
}

// Processes the image
void Vision::process(cv::Mat frame)
{
    Mat imgHsv;
    geometry_msgs::Pose2D pos;
    Scalar scalarlh[2];
    Scalar scalelow = Scalar(hsvParams[0], hsvParams[2], hsvParams[4]);
    Scalar scalehigh = Scalar(hsvParams[1], hsvParams[3], hsvParams[5]);

    scalarlh[0] = scalelow;
    scalarlh[1] = scalehigh;

    Mat imgGray;
    // Mask based on shape
    Mat shapeResult;
    cvtColor( frame, imgGray, CV_BGR2GRAY );
    blur( imgGray, imgGray, Size(3,3) );
    threshold( imgGray, imgGray, 100, 255, THRESH_BINARY );
    shapeResult = detectShapes(frame, imgGray);

    //GaussianBlur(frame, frame, Size(3,3), 0, 0); // an addition that could help reduce noise
    cvtColor(shapeResult, imgHsv, COLOR_BGR2HSV);

    // threshold the image according to given HSV parameters
    Mat colorThresh = thresholdImage(imgHsv, scalarlh);

    vector<Moments> mm = calcMoments(colorThresh);


    // Hacky solution to get ball correct, probably should use inheritance here
    if(name == "ball")
    {
        pos = getBallPose(mm);
    }
    else
    {
        pos = getRobotPose(mm);
    }

    publish(pos);
    emit processedImage(colorThresh);
}

// Thesholds the image to isolate the given colors
Mat Vision::thresholdImage(Mat& imgHSV, Scalar color[])
{
    Mat imgGray;
    inRange(imgHSV, color[0], color[1], imgGray);
    erode(imgGray, imgGray, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)));
    dilate(imgGray, imgGray, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)));
    return imgGray;
}

// Detect shapes of a chosen number of contours
Mat Vision::detectShapes(Mat frame, Mat imgGray)
{
    Mat mask(frame.rows, frame.cols, CV_8UC1, Scalar(0,0,0));
    vector< vector<Point> > contours; // vector of contours, which are vectors of points
    //Contour<Point> contours;
    vector<Vec4i> hierarchy;

    //finding all contours in the image
    //cvFindContours(imgGray, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
    findContours(imgGray, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);


    //iterating through each contour
    for(vector<Point> contour : contours)
    {
        vector<Point> result;
        //obtain a sequence of points of the countour, pointed by the variable 'countour'
        approxPolyDP(Mat(contour), result, arcLength(contour, true)*0.03, true);
        //printf("%ldu\n\r", result.size());
        //if there are 3 vertices  in the contour and the area of the triangle is more than 100 pixels
        if(result.size()==4 && fabs(contourArea(result))>100 )
        {
            //printf("in draw place yo\n\r");
            //iterating through each point
            //Point pt[3];
            //for(int i=0;i<3;i++)
            //{
            //    pt[i] = result, i);
            //}

            //drawing lines around the trianglea
            //printf("point0x: %d, point0y: %d\n\r", result[0].x, result[0].y);
            //printf("point1x: %d, point1y: %d\n\r", result[1].x, result[1].y);
            //printf("point2x: %d, point2y: %d\n\r", result[2].x, result[2].y);
            //printf("point3x: %d, point3y: %d\n\r", result[3].x, result[3].y);

            //line(frame, result[0], result[1], Scalar(255,0,0), 3);
            //line(frame, result[1], result[2], Scalar(255,0,0), 3);
            //line(frame, result[2], result[3], Scalar(255,0,0), 3);
            //line(frame, result[3], result[0], Scalar(255,0,0), 3); //1, 8, CV_AA);
            fillConvexPoly(mask, result, Scalar(255,255,255));
        }
    }
    Mat output = frame.clone().setTo(0);
    frame.copyTo(output, mask);
    return output;
}

// Given a thresholded gray image, calculate the moments in the image
vector<Moments> Vision::calcMoments(Mat imgGray)
{
    // Calculate contours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(imgGray, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    // Isolate
    vector<Moments> mm;
    for(int i = 0; i < hierarchy.size(); i++)
        mm.push_back(moments((Mat)contours[i]));

    return mm;
}

// Determines the position of a robot
geometry_msgs::Pose2D Vision::getRobotPose(vector<Moments> mm)
{
    geometry_msgs::Pose2D robotPose;

    if (mm.size() != 2)
    {
        //return imgThresholded;
        return robotPose;
    }

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
    //return imgThresholded;
    return robotPose;
}

// Determines the position of a ball
geometry_msgs::Pose2D Vision::getBallPose(vector<Moments> mm)
{
    geometry_msgs::Pose2D ballPose;

    if (mm.size() != 1)
    {
        return ballPose;
    }

    Moments moments = mm[0];
    //Moments mm = moments((Mat)contours[0]);
    Point2d ballCenter = imageToWorldCoordinates(getCenterOfMass(moments));

    ballPose.x = ballCenter.x;
    ballPose.y = ballCenter.y;
    ballPose.theta = 0;
    return ballPose;
}

// Gets the center point of a given moment
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

// Publishes the messages
void Vision::publish(geometry_msgs::Pose2D& pos)
{
    pub.publish(pos);
}

bool Vision::ok()
{
    return nh.ok();
}
