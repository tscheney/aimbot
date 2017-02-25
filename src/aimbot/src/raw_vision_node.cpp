
#include<vector>

#include "opencvsliders.h"
#include "vision.h"
#include "ros/ros.h"

#include "stdio.h"
#include <iostream>

int main(int argc, char **argv)
{
    printf("this is the beginning of the vision node\n\r");
    ros::init(argc, argv, "raw_vision");

     // create vision object
    Vision vision = Vision();
    printf("Vision object created\n\r");
    //vision.initSubscribers();

    image_transport::ImageTransport it(vision.nh);
    image_transport::Subscriber image_sub = it.subscribe("/usb_cam_away/image_raw", 1, &Vision::imageCallback, &vision);

     ros::Rate rate(100.0);
    while(vision.ok())
    {
        ros::spinOnce(); // this is where the magic happens!!
        rate.sleep();
    }
    return 0;
}