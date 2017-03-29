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

//// Update shape data
//void Vision::newShapeData(ShapeData newShapeData)
//{
//    shapeData = newShapeData;
//}

// Processes the image
void Vision::process(cv::Mat frame)
{
    geometry_msgs::Pose2D pos;

    // Mask based on shape
    Mat shapeResult = detectShapes(frame);

    // threshold the image according to given HSV parameters
    Mat colorResult = detectColors(shapeResult);

    // Calculate moments to
    vector<Moments> mm = calcMoments(colorResult);

    // Get position
    pos = getPos(mm);
    prevPos = pos;

    publish(pos);
    emit processedImage(applyMask(frame, colorResult));
}

// Apply the mask to the frame
Mat Vision::applyMask(Mat frame, Mat mask)
{
    Mat output = frame.clone().setTo(0);
    frame.copyTo(output, mask);
    return output;
}

//// Detect shapes based on the current shape data params
Mat Vision::detectShapesBase(Mat frame, int blurSize, int edgeThresh, double polyError)
{
    Mat imgGray;

    cvtColor( frame, imgGray, CV_BGR2GRAY );
    blur( imgGray, imgGray, Size(blurSize, blurSize) );
    threshold( imgGray, imgGray, edgeThresh, GlobalData::edgeThreshMax, THRESH_BINARY );

    Mat mask(frame.rows, frame.cols, CV_8UC1, Scalar(0,0,0));
    vector< vector<Point> > contours; // vector of contours, which are vectors of points
    vector<Vec4i> hierarchy;

    //finding all contours in the image
    findContours(imgGray, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    //iterating through each contour
    for(vector<Point> contour : contours)
    {
        vector<Point> result;
        approxPolyDP(Mat(contour), result, arcLength(contour, true)*polyError, true);

        if(isCorrectShape(result))
        {
            fillConvexPoly(mask, result, Scalar(255,255,255));
        }
    }

    return applyMask(frame, mask);
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
