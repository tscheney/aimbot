#ifndef VISIONTAB_H
#define VISIONTAB_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QSlider>
#include <QImage>
#include <QString>
#include <QThread>
#include <string>
#include <opencv2/core/core.hpp>
#include <cv_bridge/cv_bridge.h>
#include "vision.h"
#include "mainwindow.h"

class VisionTab : public QWidget
{
    Q_OBJECT
public:
    explicit VisionTab(QWidget *parent = 0, QString name = "default");
    ~VisionTab();
    Vision* getVision();
signals:
    void newVisionOptions(QVector<int> hsv);
public slots:
    void updateVideo(cv::Mat frame);
    void slidersChanged(int val);
private:
    Vision *vision;
    QThread visionThread;
    QVBoxLayout *layout;
    QLabel *video;
    QGroupBox *visionOptionsGroupBox;

    void setUpVision(string name);
    void setUpVideo();
    void setUpVisionOptions();
    void setUpSlider(QSlider *slider, int min, int max, int val);
    void fitVideo(cv::Mat frame);
};



#endif // VISIONTAB_H
