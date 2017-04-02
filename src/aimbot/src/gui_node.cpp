#include <QtGui/QApplication>
#include "mainwindow.h"
#include "ros/ros.h"
#include "global.h"


int main(int argc, char *argv[])
{
    ros::init(argc, argv, "raw_vision");
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<QVector<int> >("QVector<int>");
    qRegisterMetaType<ColorData>("ColorData");
    qRegisterMetaType<ShapeData>("ShapeData");
    qRegisterMetaType<RobotShapeData>("RobotShapeData");
    qRegisterMetaType<BallShapeData>("BallShapeData");

    QApplication a(argc, argv);
    MainWindow* w = new MainWindow();

    w->show();
    return a.exec();
}
