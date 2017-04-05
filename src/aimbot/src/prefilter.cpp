#include "prefilter.h"

#include <iostream>

PreFilter::PreFilter(QObject *parent) : QObject(parent)
{

}

// Process the raw frame coming in
void PreFilter::rawFrame(cv::Mat frame)
{
    cv::Mat filtered = glareReduction(frame, 170);
    emit filteredFrame(filtered);
}

// Reduce glare in the image
cv::Mat PreFilter::glareReduction(cv::Mat frame, int thresh)
{
    Mat imgGray;
    cvtColor(frame, imgGray,CV_BGR2GRAY);

    Mat imgThresh;
    threshold(imgGray, imgThresh, thresh, GlobalData::glareThreshMax,THRESH_BINARY);

    Mat dilated;
    dilate(imgThresh, dilated, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)));

    Mat inpainted;
    //inpaint(frame, imgThresh, inpainted, 20, INPAINT_TELEA);

    cvtColor(imgThresh, imgThresh, CV_GRAY2BGR);

    return frame;
}
