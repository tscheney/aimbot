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
#include <QScrollArea>
#include <string>
#include <map>
#include <opencv2/core/core.hpp>
#include <cv_bridge/cv_bridge.h>
#include "global.h"

class VisionTab : public QWidget
{
    Q_OBJECT
public:
    VisionTab(QWidget *parent = 0, QString name = "default");
    VisionTab(QWidget *parent, QString name, std::map<std::string, int>);
    ~VisionTab();
    //Vision* getVision() ;
    std::map<std::string, QSlider*> getColorSliders();
    std::map<std::string, QSlider*> getShapeSliders();
signals:
    void newColorData(ColorData colorData);
public slots:
    void updateVideo(cv::Mat frame);
    void colorSlidersChanged(int val);
    virtual void shapeSlidersChanged(int val) = 0;
protected:
    QThread visionThread;
    QVBoxLayout *layout;
    QLabel *video;
    QGroupBox *visionOptionsGroupBox;
    std::map<std::string, QSlider*> colorSliders;
    std::map<std::string, QSlider*> shapeSliders;

    virtual void setUpVision(std::string name) = 0;
    void setUpVideo();
    void setUpVisionOptions();
    void loadProfile(std::map<std::string, int> profile);
    void setUpColorOptions(QVBoxLayout *visionOptionsLayout);
    virtual void setUpShapeOptions(QVBoxLayout *visionOptionsLayout) = 0;
    void setUpSlider(QSlider *slider, int min, int max, int val);
    void fitVideo(cv::Mat frame);
};



#endif // VISIONTAB_H
