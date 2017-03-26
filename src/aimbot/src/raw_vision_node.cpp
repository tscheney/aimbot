#include <QtGui/QApplication>
#include "mainwindow.h"
#include "ros/ros.h"


int main(int argc, char *argv[])
{
    ros::init(argc, argv, "raw_vision");
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<QVector<int> >("QVector<int>");

    QApplication a(argc, argv);
    MainWindow* w = new MainWindow();

    w->show();
    return a.exec();
}
