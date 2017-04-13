#include "camlistener.h"
#include "moc_camlistener.cpp"

CamListener::CamListener(QObject *parent) //TODO: (string subPath)
 : QThread(parent), it(nh), rate(30.0)
{
    //image_sub = it.subscribe("/usb_cam_away/image_raw/", 1, boost::bind(&CamListener::imageCallback, this, _1));
    image_sub = it.subscribe("/usb_cam_away/image_raw/", 1, boost::bind(&CamListener::imageCallback, this, _1), ros::VoidPtr(), image_transport::TransportHints("compressed"));
    //image_sub = it.subscribe("/usb_cam_away/image_raw/", 1, boost::bind(&CamListener::imageCallback, this, _1), ros::VoidPtr(), image_transport::TransportHints("theora"));
    isRunning = false;
}

CamListener::~CamListener()
{
    ros::spinOnce();
    nh.shutdown();
    quit();
    wait();
}

// Threads main function to run a process
void CamListener::run()
{
    while(nh.ok() && isRunning)
    {
        ros::spinOnce();
        rate.sleep();
    }
}

// Image callback for the subscriber
void CamListener::imageCallback(const sensor_msgs::ImageConstPtr &msg)
{
    try
    {
        if(!firstFrame)
        {
            cv::Mat frame = cv_bridge::toCvShare(msg, "bgr8")->image;
            emit rawImage(frame);
        }
        else
        {
            firstFrame = false;
        }

    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

// Start stop lisenter
void CamListener::runListener(bool isRun)
{
    isRunning = isRun;
}
