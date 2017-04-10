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

    Mat applyBlur(Mat frame);
    Mat detectShapeEdges(Mat frame);
    Mat applyDilate(Mat frame);
    Mat detectShapes(Mat frame);
    bool isCorrectSize(vector<Point> shape);
    bool isCorrectShape(vector<Point> shape);
    bool isShapeLargeEnough(vector<vector<Point>> shapeResults);
    geometry_msgs::Pose2D getPos(vector<Moments> mm);
};

#endif // BALLVISION_H
