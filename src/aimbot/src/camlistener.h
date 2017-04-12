#ifndef CAMLISTENER_H
#define CAMLISTENER_H

#include <QThread>

#ifndef Q_MOC_RUN
#include "ros/ros.h"
#include <image_transport/image_transport.h>
#endif

#include <opencv2/core/core.hpp>
#include <cv_bridge/cv_bridge.h>

class CamListener : public QThread
{    Q_OBJECT
private:

    ros::NodeHandle nh;
    image_transport::ImageTransport it;
    image_transport::Subscriber image_sub;
    bool isRunning;

    void imageCallback(const sensor_msgs::ImageConstPtr &msg);
public slots:
     void runListener(bool isRun);
signals:
    // Broadcasts the subsribed frame
    void rawImage(const cv::Mat &frame);

protected:
    void run();

public:
    CamListener(QObject *parent = 0);
    ~CamListener();
};

#endif // CAMLISTENER_H
