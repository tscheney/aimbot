#include "robotvision.h"
#include "moc_robotvision.cpp"

RobotVision::RobotVision(QObject* parent, string initName) : Vision(parent, initName)
{
}

// Update shape data
void RobotVision::newShapeData(RobotShapeData newShapeData)
{
    shapeData = newShapeData;
}

// Apply blur to the image
Mat RobotVision::applyBlur(Mat frame)
{
    return applyBlurBase(frame, shapeData.blurSize);
}

// detecte edges in the image
Mat RobotVision::detectShapeEdges(Mat frame)
{
    return detectShapeEdgesBase(frame, shapeData.edgeThresh1, shapeData.edgeThresh2);
}

// Apply dilation
Mat RobotVision::applyDilate(Mat frame)
{
    return applyDilateBase(frame, shapeData.dilationIter);
}

// Detect shapes based on the current shape data params
Mat RobotVision::detectShapes(Mat frame)
{
    return detectShapesBase(frame, shapeData.blurSize, shapeData.edgeThresh1, shapeData.edgeThresh2, shapeData.polyError);
}

// Returns whether the robot is the correct size
bool RobotVision::isCorrectSize(vector<Point> shape)
{
    double area = fabs(contourArea(shape));
    bool isGreaterFrontMin = (area >= shapeData.frontMinSize);
    bool isLessFrontMax = (area <= shapeData.frontMaxSize);
    bool isGreaterBackMin = (area >= shapeData.backMinSize);
    bool isLessBackMax = (area <= shapeData.backMaxSize);
    bool isFront = isGreaterFrontMin && isLessFrontMax;
    bool isBack = isGreaterBackMin && isLessBackMax;
    return isFront || isBack;
}

// Returns whether the given shape fits the criteria for the front or back
bool RobotVision::isCorrectShape(vector<Point> shape)
{
    bool isGreaterFrontMinNumVert = (shape.size() >= shapeData.frontMinNumVert);
    bool isLessFrontMaxNumVert = (shape.size() <= shapeData.frontMaxNumVert);

    bool isGreaterBackMinNumVert = (shape.size() >= shapeData.backMinNumVert);
    bool isLessBackMaxNumVert = (shape.size() <= shapeData.backMaxNumVert);

    bool isFront = isGreaterFrontMinNumVert && isLessFrontMaxNumVert;
    bool isBack = isGreaterBackMinNumVert && isLessBackMaxNumVert;

    return isFront || isBack;
}

// Return if the shape results vector is large enough
bool RobotVision::isShapeLargeEnough(vector<vector<Point>> shapeResults)
{
    return shapeResults.size() >= 2;
}

// Determines the position of a robot
geometry_msgs::Pose2D RobotVision::getPos(vector<Moments> mm)
{
    geometry_msgs::Pose2D robotPos;

    if (mm.size() < 2)
    {
        return prevPos;
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
    robotPos.x = robotCenter.x;
    robotPos.y = robotCenter.y;
    robotPos.theta = angle;

    if(!isnan(robotPos.x) && !isnan(robotPos.y) && !isnan(robotPos.theta))
    {
        return robotPos;
    }
    else
    {
        return prevPos;
    }

}
