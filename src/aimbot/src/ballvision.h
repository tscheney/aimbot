#ifndef BALLVISION_H
#define BALLVISION_H

#include "vision.h"

class BallVision : public Vision
{
    Q_OBJECT
public:
    BallVision(QObject* parent, string initName);
public slots:
    void newShapeData(BallShapeData newShapeData);
private:
    BallShapeData shapeData;

    Mat detectShapes(Mat frame);
    bool isCorrectShape(vector<Point> shape);
    geometry_msgs::Pose2D getPos(vector<Moments> mm);
};

#endif // BALLVISION_H
