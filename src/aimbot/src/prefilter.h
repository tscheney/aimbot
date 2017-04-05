#ifndef PREFILTER_H
#define PREFILTER_H

#include <QObject>
#include <opencv2/imgproc/imgproc.hpp>
#include "global.h"
#include <opencv2/photo/photo.hpp>

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

private:
    cv::Mat glareReduction(cv::Mat frame, int thresh);
};

#endif // PREFILTER_H
