
#include<vector>

#include "opencvsliders.h"
#include "vision.h"
#include "ros/ros.h"


int main(int argc, char **argv)
{
    ros::init(argc, argv, "raw_vision");

     // create vision object
    Vision vision = Vision();

    ros::spin();
    return 0;
}