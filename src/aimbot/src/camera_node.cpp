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

vector<Mat> loadImages()
{
    vector<Mat> images;
    for(int i = 2; i <= 12; i++)
    {
        string filename = "/home/tom/Downloads/ss/image" + to_string(i) + ".png";
        //string filename = "/home/tom/Downloads/ss/image1.png";
        Mat image = imread(filename);
        images.push_back(image);
    }
    return images;
}

// This is a debugging tool that publishes the webcam feed to the same node as used in the robot soccer lab
int main(int argc, char** argv)
{
    bool webCam = false;
    Mat frame;

    VideoCapture capture(CV_CAP_ANY);

    vector<Mat> images = loadImages();
    int imageNum = 0;
    int frameNum = 0;

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
                if(frameNum < 60)
                {
                    frameNum++;
                }
                else
                {
                    frameNum = 0;
                    if(imageNum != 10)
                    {
                        imageNum++;
                    }
                    else
                    {
                        imageNum = 0;
                    }
                }
                printf("imageNum: %d\n\r", imageNum + 2);
                frame = images.at(imageNum);
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
