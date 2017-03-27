
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
    name = initName;
    initPublishers(name);
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

// Update color data
void Vision::newColorData(ColorData newColorData)
{
    colorData = newColorData;
}

// Update shape data
void Vision::newShapeData(ShapeData newShapeData)
{
    shapeData = newShapeData;
}

// Processes the image
void Vision::process(cv::Mat frame)
{
    geometry_msgs::Pose2D pos;

    // Mask based on shape
    Mat shapeResult = detectShapes(frame);

    // threshold the image according to given HSV parameters
    //Mat colorThresh = detectColors(shapeResult);

    // Calculate moments to
    //vector<Moments> mm = calcMoments(colorThresh);


    // Hacky solution to get ball correct, probably should use inheritance here
    if(name == "ball")
    {
        //pos = getBallPose(mm);
    }
    else
    {
        //pos = getRobotPose(mm);
    }

    publish(pos);
    emit processedImage(shapeResult);
}

// Detect shapes based on the current shape data params
Mat Vision::detectShapes(Mat frame)
{
    std::cout << shapeData.toString();
    Mat imgGray;

    cvtColor( frame, imgGray, CV_BGR2GRAY );
    blur( imgGray, imgGray, Size(shapeData.blurSize,shapeData.blurSize) );
    threshold( imgGray, imgGray, shapeData.edgeThresh, GlobalData::edgeThreshMax, THRESH_BINARY );

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
        approxPolyDP(Mat(contour), result, arcLength(contour, true)*shapeData.polyError, true);
        //printf("%ldu\n\r", result.size());
        //if there are 3 vertices  in the contour and the area of the triangle is more than 100 pixels
        if(isCorrectShape(result))
        {
            printf("correct shape\n\r");
            fillConvexPoly(mask, result, Scalar(255,255,255));
        }
    }
    Mat output = frame.clone().setTo(0);
    frame.copyTo(output, mask);
    return output;
}

// Returns whether the given shape fits the criteria for the front or back
bool Vision::isCorrectShape(vector<Point> shape)
{
    double area = fabs(contourArea(shape));
    bool isFrontNumVert = (shape.size() == shapeData.frontNumVert);
    bool isGreaterFrontMin = (area >= shapeData.frontMinSize);
    bool isLessFrontMax = (area <= shapeData.frontMaxSize);
    bool isBackNumVert = (shape.size() == shapeData.backNumVert);
    bool isGreaterBackMin = (area >= shapeData.backMinSize);
    bool isLessBackMax = (area <= shapeData.backMaxSize);
    bool isFront = isFrontNumVert && isGreaterFrontMin && isLessFrontMax;
    bool isBack = isBackNumVert && isGreaterBackMin && isLessBackMax;
    return isFront || isBack;
}

// Detect the HSV color range based on the current color data params
Mat Vision::detectColors(Mat frame)
{
    Mat imgHSV;

    Scalar scalarlh[2];
    Scalar scalelow = Scalar(colorData.hLow, colorData.sLow, colorData.vLow);
    Scalar scalehigh = Scalar(colorData.hHigh, colorData.sHigh, colorData.vHigh);

    scalarlh[0] = scalelow;
    scalarlh[1] = scalehigh;

    cvtColor(frame, imgHSV, COLOR_BGR2HSV);

    Mat imgGray = thresholdImage(imgHSV, scalarlh);

    return imgGray;
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
