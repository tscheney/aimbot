#include "ballvision.h"
#include "moc_ballvision.cpp"

BallVision::BallVision(QObject* parent, string initName) : Vision(parent, initName)
{
}

// Update shape data
void BallVision::newShapeData(BallShapeData newShapeData)
{
    shapeData = newShapeData;
}

// Detect shapes based on the current shape data params
Mat BallVision::detectShapes(Mat frame)
{
    return detectShapesBase(frame, shapeData.blurSize, shapeData.edgeThresh, shapeData.polyError);
}

// Returns whether the given shape fits the criteria for the ball
bool BallVision::isCorrectShape(vector<Point> shape)
{
    bool isGreaterMinNumVert = (shape.size() >= shapeData.minNumVert);
    bool isLessMaxNumVert = (shape.size() <= shapeData.maxNumVert);

    return isGreaterMinNumVert && isLessMaxNumVert;
}

// Returns whether the given shape fits the criteria for the ball
bool BallVision::isCorrectSize(vector<Point> shape)
{
    double area = fabs(contourArea(shape));
    bool isGreaterMinSize = (area >= shapeData.minSize);
    bool isLessMaxSize = (area <= shapeData.maxSize);

    return isGreaterMinSize && isLessMaxSize;
}

// Return if the shape result is large enough
bool BallVision::isShapeLargeEnough(vector<vector<Point>> shapeResults)
{
    return shapeResults.size() >= 1;
}

// Determines the position of a ball
geometry_msgs::Pose2D BallVision::getPos(vector<Moments> mm)
{
    geometry_msgs::Pose2D ballPos;

    if (mm.size() != 1)
    {
        return prevPos;
    }

    Moments moments = mm[0];
    //Moments mm = moments((Mat)contours[0]);
    Point2d ballCenter = imageToWorldCoordinates(getCenterOfMass(moments));

    ballPos.x = ballCenter.x;
    ballPos.y = ballCenter.y;
    ballPos.theta = 0;

    return ballPos;
}
