#include "prefilter.h"

#include <iostream>

PreFilter::PreFilter(QObject *parent) : QObject(parent)
{
    pMOG2 = createBackgroundSubtractorMOG2(1000, 16, false);
}

// Process the raw frame coming in
void PreFilter::rawFrame(cv::Mat frame)
{
    Mat filtered = backgroundSubtraction(frame);
    emit filteredFrame(filtered);
    //filteredFrame(frame);
}

// Reduce glare in the image
Mat PreFilter::glareReduction(cv::Mat frame, int thresh)
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

// Do background subtraction
Mat PreFilter::backgroundSubtraction(cv::Mat frame)
{
    pMOG2->apply(frame, fgMaskMOG2);
    return applyMask(frame, fgMaskMOG2);
}

// Apply the mask to the frame
Mat PreFilter::applyMask(cv::Mat frame, cv::Mat mask)
{
    Mat output = frame.clone().setTo(0);
    frame.copyTo(output, mask);
    return output;
}
