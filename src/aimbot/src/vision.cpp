#include "vision.h"
#include "moc_vision.cpp"

float Vision::FIELD_WIDTH = 3.53;  // in meters
float Vision::FIELD_HEIGHT = 2.39; 
float Vision::ROBOT_RADIUS = 0.10;
float Vision::FIELD_WIDTH_PIXELS = 610.0; // measured from threshold of goal to goal
float Vision::FIELD_HEIGHT_PIXELS = 426.0; // measured from inside of wall to wall
float Vision::FIELD_X_OFFSET = 111;
float Vision::FIELD_Y_OFFSET = 17;
float Vision::CAMERA_WIDTH = 864.0; //Todo check this it is likely that it is 848
float Vision::CAMERA_HEIGHT = 480.0;

Vision::Vision(QObject* parent, string initName, bool inIsHome) : QObject(parent)
{
    name = initName;
    isHome = inIsHome;
    initPublishers();
}

// Initialize the publishers this class publishes
void Vision::initPublishers()
{
    string teamSide = "home";
    if(!isHome)
    {
        teamSide = "away";
    }
    string space = GlobalData::spacePrefix + teamSide + "/raw_vision/";
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

    Mat result;

    //
    if(isUseFieldMask)
    {
        Mat mask = maskField(frame);
        result = applyMask(frame.clone(), mask);
    }
    else
    {
        result = frame.clone();
    }

    if(isUseGray)
    {
        cout << "isUseGray\n\r";
        Mat imgGray;
        Mat imgResult;
        cvtColor(frame, imgGray, COLOR_BGR2GRAY);
        cvtColor(imgGray, imgResult, COLOR_GRAY2BGR);
        result = imgResult;
        frame = imgResult;
    }

    result = applyBlur(result);

    if(isUseEdgeDetect)
    {
        cout << "isUseEdgeDetect\n\r";
        result = detectShapeEdges(result);
    }

    if(isUseEdgeDetect)
    {
        result = applyDilate(result);
    }

    // Mask based on shape
    if(isUseShape)
    {
        cout << "isUseShape\n\r";
        if(!isUseEdgeDetect) // && !isUseColor)
        {
            cvtColor(frame, result, COLOR_BGR2GRAY);
        }
        result = detectShapes(result);
    }

    if(isUseColor)
    {
        if(isUseEdgeDetect || isUseShape)
        {
            // apply mask to restore color
            result = applyMask(frame.clone(), result.clone());
        }
        // threshold the image according to given HSV parameters
        result = detectColors(result);
    }

    // Calculate moments to find position
    vector<Moments> mm;

    if(isUseColor || isUseEdgeDetect || isUseShape)
    {
        mm = calcMoments(result);
    }

    // Get position
    pos = getPos(mm);
    prevPos = pos;

    publish(pos);
    emit processedImage(applyMask(frame, result));
    //emit processedImage(result);
}

// A new unfiltered frame has arrived
void Vision::newUnfilteredFrame(Mat frame)
{
    if(!isUseBackSub)
    {
        process(frame);
    }
}

// A new prefiltered frame has arrived
void Vision::newPrefiltFrame(cv::Mat frame)
{
    if(isUseBackSub)
    {
        process(frame);
    }
}

// Apply the mask to the frame
Mat Vision::applyMask(Mat frame, Mat mask)
{
    Mat output = frame.clone().setTo(0);
    frame.copyTo(output, mask);
    return output;
}

// Apply a Gaussian blur of size blursize
Mat Vision::applyBlurBase(Mat frame, int blurSize)
{
    Mat result(frame.size(), frame.type());
    GaussianBlur(frame, result, Size(blurSize, blurSize), 0, 0);
    //blur(result, result, Size(blurSize, blurSize));
    return result;
    //return frame;
}

// Detect shapes based on the current shape data params
Mat Vision::detectShapesBase(Mat frame, int blurSize, int edgeThresh1, int edgeThresh2, double polyError)
{
    Mat imgGray = frame;

    vector< vector<Point> > contours; // vector of contours, which are vectors of points
    vector<Vec4i> hierarchy;

    //finding all contours in the image
    findContours(imgGray, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    vector<vector<Point>> sizeResults;
    vector<vector<Point>> shapeResults;
    //iterating through each contour
    for(vector<Point> contour : contours)
    {
        vector<Point> result;
        approxPolyDP(Mat(contour), result, arcLength(contour, true)*polyError, false);

        if(isCorrectSize(result)) // check for correct size
        {
            sizeResults.push_back(result);
            if(isCorrectShape(result)) // check for correct shape
            {
                shapeResults.push_back(result);
            }
        }
    }
    //    Mat mask(frame.rows, frame.cols, CV_8UC1, Scalar(0,0,0));
    Mat mask = getShapeMask(frame, sizeResults, shapeResults);

    return mask;
}

// Use canny edge detection to detect edges
Mat Vision::detectShapeEdgesBase(Mat frame, int edgeThresh1, int edgeThresh2)
{
    Mat edges(frame.size(), frame.type());
    Canny(frame, edges, edgeThresh1, edgeThresh2, 3, true);
    return edges;
}

// Dilate the image
Mat Vision::applyDilateBase(Mat frame, int iterations)
{
    Mat dilated(frame.size(), frame.type());
    dilate(frame, dilated, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)), Point(-1,-1) , iterations);
    return dilated;
}

// get the shape mask
Mat Vision::getShapeMask(Mat frame, vector<vector<Point>> sizeResults, vector<vector<Point>> shapeResults)
{
    Mat mask(frame.rows, frame.cols, CV_8UC1, Scalar(0,0,0));
    vector<vector<Point>> results;
    if(isShapeLargeEnough(shapeResults))
    {
        results = shapeResults;
    }
    else
    {
        results = sizeResults;
    }

    // add result shapes to mask
    for(vector<Point> result : results)
    {
        fillConvexPoly(mask, result, Scalar(255,255,255));
    }

    return mask;
}

// Detect the HSV color range based on the current color data params
Mat Vision::detectColors(Mat frame)
{
    Mat imgHSV;

    cvtColor(frame, imgHSV, COLOR_BGR2HSV);

    Mat imgGray;

    if(colorData.hLow <= colorData.hHigh)
    {
        imgGray = colorThreshold(imgHSV);
    }
    else // low h value is greater than high, use wrap around
    {
        imgGray = colorThresholdWrap(imgHSV);
    }

    return imgGray;
}

// Thesholds the image to isolate the given colors
Mat Vision::colorThreshold(Mat& imgHSV)
{
    Mat imgGray;
    inRange(imgHSV, Scalar(colorData.hLow, colorData.sLow, colorData.vLow),
            Scalar(colorData.hHigh, colorData.sHigh, colorData.vHigh), imgGray);
    imgGray = erodeDilate(imgGray);
    return imgGray;
}

// Thresholds the image using iterations to cover a wrap around h
Mat Vision::colorThresholdWrap(Mat &imgHSV)
{
    Mat1b mask1, mask2;
    inRange(imgHSV, Scalar(0, colorData.sLow, colorData.vLow),
                Scalar(colorData.hHigh, colorData.sHigh, colorData.vHigh),mask1);
    inRange(imgHSV, Scalar(colorData.hLow, colorData.sLow, colorData.vLow),
                Scalar(GlobalData::colorHMax, colorData.sHigh, colorData.vHigh),mask2);
    Mat1b mask = mask1 | mask2;
    mask = erodeDilate(mask);
    return mask;
}

// Erodes and dilates the image
Mat Vision::erodeDilate(Mat& frame)
{
    Mat eroded;
    erode(frame, eroded, getStructuringElement(MORPH_ELLIPSE, Size(colorData.erosDilaSize, colorData.erosDilaSize)), Point(-1,-1), colorData.erosionIter);
    Mat dilated;
    dilate(eroded, dilated, getStructuringElement(MORPH_ELLIPSE, Size(colorData.erosDilaSize, colorData.erosDilaSize)), Point(-1,-1),colorData.dilationIter);
    return dilated;
}

// Mask so that only the field is visible
Mat Vision::maskField(Mat frame)
{
    //Mat mask(frame.size(), frame.type());
    Mat mask = Mat::zeros(frame.rows, frame.cols, CV_8U);
    fillConvexPoly(mask, GlobalData::fieldPoints, Scalar(1));
    return(mask);
    //return applyMask(frame, mask);
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

// Recieve new use background subtraction value
void Vision::useBackSub(bool value)
{
    isUseBackSub = value;
}

// Use the field mask
void Vision::newFieldMask(bool value)
{
    isUseFieldMask = value;
}

// Recieve new use color value
void Vision::useColor(bool value)
{
    isUseColor = value;
}

// Recieve new use edge detect value
void Vision::useEdgeDetect(bool value)
{
    isUseEdgeDetect = value;
}

// Recieve new use shape value
void Vision::useShape(bool value)
{
    isUseShape = value;
}

// Handle new team side
void Vision::newTeamSide(bool inIsHome)
{
    isHome = inIsHome;
}

// Handle use gray
void Vision::useGray(bool isGray)
{
    isUseGray = isGray;
}

bool Vision::ok()
{
    return nh.ok();
}
