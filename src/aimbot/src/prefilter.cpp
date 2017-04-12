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

// Handle recieved calc back sub event
void PreFilter::calcBackSub(bool isCalc)
{
    isCalcBackSub = isCalc;
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
    if(isCalcBackSub)
    {
        pMOG2->apply(frame, fgMaskMOG2);
        return applyMask(frame, fgMaskMOG2);
    }
    else
    {
        //Mat invert;
        //bitwise_not(frame, invert);
//        Mat imgHSV;
//        cvtColor(frame, imgHSV, COLOR_BGR2HSV);

//        std::vector<Mat> splitHSV(3);
//        split(imgHSV, splitHSV);
//        Mat hue = splitHSV.at(0);
//        Mat sat = splitHSV.at(1);
//        Mat val = splitHSV.at(2);
//        hue = Scalar::all(128);
//        //sat = Scalar::all(255);
//        //val = Scalar::all(255);
//        std::vector<Mat> newHSV;
//        newHSV.push_back(hue);
//        newHSV.push_back(sat);
//        newHSV.push_back(val);
//        Mat newFrame;
//        merge(newHSV, newFrame);
//        Mat bgrFrame;
//        cvtColor(newFrame, bgrFrame, COLOR_HSV2BGR);

        //Mat imgGray;
        //Mat imgBGRGray;
        //cvtColor(frame, imgGray, COLOR_BGR2GRAY);
        //cvtColor(imgGray, imgBGRGray, COLOR_GRAY2BGR);


        return frame;
    }
}

// Apply the mask to the frame
Mat PreFilter::applyMask(cv::Mat frame, cv::Mat mask)
{
    Mat output = frame.clone().setTo(0);
    frame.copyTo(output, mask);
    return output;
}
