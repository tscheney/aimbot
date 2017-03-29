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

// Detect shapes based on the current shape data params
Mat RobotVision::detectShapes(Mat frame)
{
    return detectShapesBase(frame, shapeData.blurSize, shapeData.edgeThresh, shapeData.polyError);
}

// Returns whether the given shape fits the criteria for the front or back
bool RobotVision::isCorrectShape(vector<Point> shape)
{
    double area = fabs(contourArea(shape));
    bool isGreaterFrontMinNumVert = (shape.size() >= shapeData.frontMinNumVert);
    bool isLessFrontMaxNumVert = (shape.size() <= shapeData.frontMaxNumVert);
    bool isGreaterFrontMin = (area >= shapeData.frontMinSize);
    bool isLessFrontMax = (area <= shapeData.frontMaxSize);
    bool isGreaterBackMinNumVert = (shape.size() >= shapeData.backMinNumVert);
    bool isLessBackMaxNumVert = (shape.size() <= shapeData.backMaxNumVert);
    bool isGreaterBackMin = (area >= shapeData.backMinSize);
    bool isLessBackMax = (area <= shapeData.backMaxSize);
    bool isFront = isGreaterFrontMinNumVert && isLessFrontMaxNumVert && isGreaterFrontMin && isLessFrontMax;
    bool isBack = isGreaterBackMinNumVert && isLessBackMaxNumVert && isGreaterBackMin && isLessBackMax;
    return isFront || isBack;
}

// Determines the position of a robot
geometry_msgs::Pose2D RobotVision::getPos(vector<Moments> mm)
{
    geometry_msgs::Pose2D robotPos;

    if (mm.size() != 2)
    {
        return robotPos;
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

    return robotPos;
}
