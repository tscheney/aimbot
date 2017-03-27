#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

// This is a debugging tool that publishes the webcam feed to the same node as used in the robot soccer lab
int main(int argc, char** argv)
{
    bool webCam = false;
    Mat frame;

    VideoCapture capture(CV_CAP_ANY);

    Mat image = imread("/home/tom/Downloads/shapes.jpg");

    ros::init(argc, argv, "camera");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise("/usb_cam_away/image_raw", 1);

    cv::waitKey(30);
    sensor_msgs::ImagePtr msg;

    ros::Rate loop_rate(60);
    while (nh.ok())
    {
        if (capture.isOpened())
        {
            if (webCam)
            {
                capture.read(frame);
            }
            else
            {
                frame = image;
            }

            msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
            pub.publish(msg);
            cv::waitKey(1);
        }

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
