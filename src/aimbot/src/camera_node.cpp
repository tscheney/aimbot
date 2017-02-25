#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    printf("this is the beginning of the camera node\n\r");
    Mat frame;

    VideoCapture capture(CV_CAP_ANY);

    ros::init(argc, argv, "camera");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise("/usb_cam_away/image_raw", 1);
    //cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    cv::waitKey(30);
    sensor_msgs::ImagePtr msg;

    ros::Rate loop_rate(60);
    while (nh.ok())
    {
        if (capture.isOpened())
        {
            capture.read(frame);
            msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
            pub.publish(msg);
            cv::waitKey(1);
        }

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}