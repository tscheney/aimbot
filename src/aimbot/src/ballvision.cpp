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

// Apply blur to the image
Mat BallVision::applyBlur(Mat frame)
{
    return applyBlurBase(frame, shapeData.blurSize);
}

// detecte edges in the image
Mat BallVision::detectShapeEdges(Mat frame)
{
    return detectShapeEdgesBase(frame, shapeData.edgeThresh1, shapeData.edgeThresh2);
}

Mat BallVision::applyDilate(Mat frame)
{
    return applyDilateBase(frame, shapeData.dilationIter);
}

// Detect shapes based on the current shape data params
Mat BallVision::detectShapes(Mat frame)
{
    return detectShapesBase(frame, shapeData.blurSize, shapeData.edgeThresh1, shapeData.edgeThresh2, shapeData.polyError);
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

    if (mm.size() < 1)
    {
        cout << "prev pos\n";
        return prevPos;
    }

    Moments moments = mm[mm.size() - 1]; // grab the largest moment
    //Moments mm = moments((Mat)contours[0]);
    Point2d ballCenter = imageToWorldCoordinates(getCenterOfMass(moments));

    ballPos.x = ballCenter.x;
    ballPos.y = ballCenter.y;
    ballPos.theta = 0;
    if(!isnan(ballPos.x) && !isnan(ballPos.y) && !isnan(ballPos.theta))
    {
        return ballPos;
    }
    else
    {
        return prevPos;
    }
}
