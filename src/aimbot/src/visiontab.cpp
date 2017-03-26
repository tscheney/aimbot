#include "visiontab.h"
#include "moc_visiontab.cpp"

VisionTab::VisionTab(QWidget *parent, QString name) : QWidget(parent)
{
    setUpVision(name.toStdString());
    layout = new QVBoxLayout();
    setUpVideo();
    setUpVisionOptions();
    setLayout(layout);
}

VisionTab::~VisionTab()
{
    //Todo Delete everything
    visionThread.quit();
    visionThread.wait();
    delete layout;
}

// Set up the vision processing
void VisionTab::setUpVision(string name)
{
    vision = new Vision(0, name);
    connect(vision, SIGNAL(processedImage(cv::Mat)), this, SLOT(updateVideo(cv::Mat)));
    connect(this, SIGNAL(newVisionOptions(QVector<int>)), vision, SLOT(newHSV(QVector<int>)));
    vision->moveToThread(&visionThread);
    visionThread.start();
}

void VisionTab::setUpVideo()
{
    video = new QLabel();
    layout->addWidget(video);
}

// Set up the vision options group box
void VisionTab::setUpVisionOptions()
{
    visionOptionsGroupBox = new QGroupBox(tr("Vision Options"));
    QFormLayout *visionOptionsLayout = new QFormLayout();

    // Labels
    QLabel *hLowLabel = new QLabel(tr("H Low:"));
    QLabel *hHighLabel = new QLabel(tr("H High:"));
    QLabel *sLowLabel = new QLabel(tr("S Low:"));
    QLabel *sHighLabel = new QLabel(tr("S High:"));
    QLabel *vLowLabel = new QLabel(tr("V Low:"));
    QLabel *vHighLabel = new QLabel(tr("V High:"));

    // Sliders
    const int min = 0;
    const int hMax = 179;
    const int svMax = 255;
    QSlider *hLowSlider = new QSlider(Qt::Horizontal);
    setUpSlider(hLowSlider, min, hMax, min);
    QSlider *hHighSlider = new QSlider(Qt::Horizontal);
    setUpSlider(hHighSlider, min, hMax, hMax);
    QSlider *sLowSlider = new QSlider(Qt::Horizontal);
    setUpSlider(sLowSlider, min, svMax, min);
    QSlider *sHighSlider = new QSlider(Qt::Horizontal);
    setUpSlider(sHighSlider, min, svMax, svMax);
    QSlider *vLowSlider = new QSlider(Qt::Horizontal);
    setUpSlider(vLowSlider, min, svMax, min);
    QSlider *vHighSlider = new QSlider(Qt::Horizontal);
    setUpSlider(vHighSlider, min, svMax, svMax);

    // Connect Sliders
    connect(hLowSlider, SIGNAL(valueChanged(int)), this, SLOT(slidersChanged(int)));
    connect(hHighSlider, SIGNAL(valueChanged(int)), this, SLOT(slidersChanged(int)));
    connect(sLowSlider, SIGNAL(valueChanged(int)), this, SLOT(slidersChanged(int)));
    connect(sHighSlider, SIGNAL(valueChanged(int)), this, SLOT(slidersChanged(int)));
    connect(vLowSlider, SIGNAL(valueChanged(int)), this, SLOT(slidersChanged(int)));
    connect(vHighSlider, SIGNAL(valueChanged(int)), this, SLOT(slidersChanged(int)));

    // Add rows to Form Layout
    visionOptionsLayout->addRow(hLowLabel, hLowSlider);
    visionOptionsLayout->addRow(hHighLabel, hHighSlider);
    visionOptionsLayout->addRow(sLowLabel, sLowSlider);
    visionOptionsLayout->addRow(sHighLabel, sHighSlider);
    visionOptionsLayout->addRow(vLowLabel, vLowSlider);
    visionOptionsLayout->addRow(vHighLabel, vHighSlider);

    visionOptionsGroupBox->setLayout(visionOptionsLayout);
    layout->addWidget(visionOptionsGroupBox);
}

// Set up individual sliders
void VisionTab::setUpSlider(QSlider *slider, int min, int max, int val)
{
    slider->setMinimum(min);
    slider->setMaximum(max);
    slider->setValue(val);
}

// Updates the video with a new frame
void VisionTab::updateVideo(cv::Mat frame)
{
    fitVideo(frame);
    cv::Mat RGBframe;
    QImage img;
    if (frame.channels()== 3){
        cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
        img = QImage((const unsigned char*)(RGBframe.data),
                          RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
    }
    else
    {
        img = QImage((const unsigned char*)(frame.data),
                             frame.cols,frame.rows,QImage::Format_Indexed8);
    }

    if (!img.isNull())
    {
        video->setAlignment(Qt::AlignCenter);
        video->setPixmap(QPixmap::fromImage(img).scaled(video->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
    }
}

// Change video size to match the frame size
void VisionTab::fitVideo(cv::Mat frame)
{
    if(frame.rows != video->geometry().height())
    {

        QRect newHeight = video->geometry();
        newHeight.setHeight(frame.rows);
        video->setGeometry(newHeight);
    }
    if(frame.cols != video->geometry().width())
    {
        QRect newWidth = video->geometry();
        newWidth.setWidth(frame.cols);
        video->setGeometry(newWidth);
    }
}

// Return a pointer to the vision data member
Vision  *VisionTab::getVision()
{
    return vision;
}

// Send out all values from sliders
void VisionTab::slidersChanged(int val)
{
    QVector<int> hsv;
    QList<QSlider *> sliderList = visionOptionsGroupBox->findChildren<QSlider *>();
    for(QSlider *slider : sliderList)
    {
        //printf("%d\n\r", slider->value());
        hsv.push_back(slider->value());
    }
    emit newVisionOptions(hsv);
}
