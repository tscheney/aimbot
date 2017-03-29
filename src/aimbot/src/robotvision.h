#ifndef ROBOTVISION_H
#define ROBOTVISION_H

#include "vision.h"

class RobotVision : public Vision
{
    Q_OBJECT
public:
    RobotVision(QObject* parent, string initName);
public slots:
    void newShapeData(RobotShapeData newShapeData);
private:
    RobotShapeData shapeData;

    Mat detectShapes(Mat frame);
    bool isCorrectShape(vector<Point> shape);
    geometry_msgs::Pose2D getPos(vector<Moments> mm);
};

#endif // ROBOTVISION_H
