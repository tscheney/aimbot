#ifndef PREFILTER_H
#define PREFILTER_H

#include <QObject>
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>
#include "global.h"
#include <opencv2/photo/photo.hpp>
#include <opencv2/video/background_segm.hpp>

using namespace cv;

class PreFilter : public QObject
{
    Q_OBJECT
public:
    PreFilter(QObject *parent = 0);

signals:
    void filteredFrame(cv::Mat frame);

public slots:
    void rawFrame(cv::Mat frame);
    void calcBackSub(bool isCalc);

private:
    Mat glareReduction(cv::Mat frame, int thresh);
    Mat backgroundSubtraction(cv::Mat frame);
    Mat applyMask(cv::Mat frame, cv::Mat mask);

    Ptr<BackgroundSubtractor> pMOG2;
    Mat fgMaskMOG2;

    bool isCalcBackSub = true;
};

#endif // PREFILTER_H
