#include "visiontab.h"
#include "moc_visiontab.cpp"
#include <iostream>

VisionTab::VisionTab(QWidget *parent, QString name) : QWidget(parent)
{
//    setUpVision(name.toStdString());
//    layout = new QVBoxLayout();
//    setUpVideo();
//    setUpVisionOptions();
//    setLayout(layout);
}

VisionTab::VisionTab(QWidget *parent, QString name, std::map<std::string, int> profile) : QWidget(parent)
{
//    setUpVision(name.toStdString());
//    layout = new QVBoxLayout();
//    setUpVideo();
//    setUpVisionOptions();
//    setLayout(layout);
//    Profile(profile);
}

VisionTab::~VisionTab()
{
    //Todo Delete everything
    visionThread.quit();
    visionThread.wait();
    delete layout;
}

//// Set up the vision processing
//void VisionTab::setUpVision(string name)
//{
//    vision = new RobotVision(0, name);
//    connect(vision, SIGNAL(processedImage(cv::Mat)), this, SLOT(updateVideo(cv::Mat)));
//    connect(this, SIGNAL(newColorData(ColorData)), vision, SLOT(newColorData(ColorData)));
//    connect(this, SIGNAL(newShapeData(RobotShapeData)), vision, SLOT(newShapeData(RobotShapeData)));
//    vision->moveToThread(&visionThread);
//    visionThread.start();
//}

void VisionTab::setUpVideo()
{
    video = new QLabel();
    layout->addWidget(video);
}

// Set up the vision options group box
void VisionTab::setUpVisionOptions()
{
    visionOptionsGroupBox = new QGroupBox(tr("Vision Options"));

    QVBoxLayout *visionOptionsLayout = new QVBoxLayout();


    QLabel *useBackSubLabel = new QLabel(tr("Use Background Subtraction"));
    QCheckBox *useBackSubCheckBox = new QCheckBox();
    useBackSubCheckBox->setChecked(true);
    connect(useBackSubCheckBox, SIGNAL(toggled(bool)), this, SLOT(useBackSubChanged(bool)));
    visionOptionsLayout->addWidget(useBackSubLabel);
    visionOptionsLayout->addWidget(useBackSubCheckBox);

    QLabel *grayLabel = new QLabel(tr("Grayscale"));
    QCheckBox *grayCheckBox = new QCheckBox();
    grayCheckBox->setChecked(false);
    connect(grayCheckBox, SIGNAL(toggled(bool)), this, SLOT(useGrayChanged(bool)));
    visionOptionsLayout->addWidget(grayLabel);
    visionOptionsLayout->addWidget(grayCheckBox);

    setUpColorOptions(visionOptionsLayout);
    setUpShapeOptions(visionOptionsLayout);

    visionOptionsGroupBox->setLayout(visionOptionsLayout);
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(visionOptionsGroupBox);
    layout->addWidget(scrollArea);
}

// Loads the given profile
void VisionTab::loadProfile(std::map<std::string, int> profile)
{
    for (auto const& element : profile)
    {
        if(colorSliders.count(element.first))
        {
            colorSliders.at(element.first)->setValue(element.second);
        }

        if(shapeSliders.count(element.first))
        {
            shapeSliders.at(element.first)->setValue(element.second);
        }
    }
}

// Set up the color options sliders
void VisionTab::setUpColorOptions(QVBoxLayout *visionOptionsLayout)
{
    QGroupBox *colorOptionsGroupBox = new QGroupBox(tr("Color Options"));
    colorOptionsGroupBox->setObjectName("Color Options");
    QFormLayout *colorOptionsLayout = new QFormLayout();

    // Labels
    QLabel *hLowLabel = new QLabel(tr("H Low:"));
    QLabel *hHighLabel = new QLabel(tr("H High:"));
    QLabel *sLowLabel = new QLabel(tr("S Low:"));
    QLabel *sHighLabel = new QLabel(tr("S High:"));
    QLabel *vLowLabel = new QLabel(tr("V Low:"));
    QLabel *vHighLabel = new QLabel(tr("V High:"));\
    QLabel *erosionIterLabel = new QLabel(tr("Erosion Iterations:"));
    QLabel *dilationIterLabel = new QLabel(tr("Dilate Iterations:"));
    QLabel *erosDilaSizeLabel = new QLabel(tr("Erosion/Dilation Size:"));

    QLabel *useColorLabel = new QLabel(tr("Use Color"));
    QCheckBox *useColorCheckBox = new QCheckBox();
    useColorCheckBox->setChecked(true);
    connect(useColorCheckBox, SIGNAL(toggled(bool)), this, SLOT(useColorChanged(bool)));
    colorOptionsLayout->addRow(useColorLabel, useColorCheckBox);

    // Sliders
    const int min = 0;
    const int hMax = GlobalData::colorHMax;
    const int svMax = GlobalData::colorSVMax;
    QSlider *hLowSlider = new QSlider(Qt::Horizontal);
    setUpSlider(hLowSlider, min, hMax, min);
    //hLowSlider->setObjectName("hLowSlider");
    colorSliders.insert(std::pair<std::string, QSlider*>("hLow", hLowSlider));

    QSlider *hHighSlider = new QSlider(Qt::Horizontal);
    setUpSlider(hHighSlider, min, hMax, hMax);
    //hHighSlider->setObjectName("hHighSlider");
    colorSliders.insert(std::pair<std::string, QSlider*>("hHigh", hHighSlider));

    QSlider *sLowSlider = new QSlider(Qt::Horizontal);
    setUpSlider(sLowSlider, min, svMax, min);
    //sLowSlider->setObjectName("sLowSlider");
    colorSliders.insert(std::pair<std::string, QSlider*>("sLow", sLowSlider));

    QSlider *sHighSlider = new QSlider(Qt::Horizontal);
    setUpSlider(sHighSlider, min, svMax, svMax);
    //sHighSlider->setObjectName("sHighSlider");
    colorSliders.insert(std::pair<std::string, QSlider*>("sHigh", sHighSlider));

    QSlider *vLowSlider = new QSlider(Qt::Horizontal);
    setUpSlider(vLowSlider, min, svMax, min);
    //vLowSlider->setObjectName("vLowSlider");
    colorSliders.insert(std::pair<std::string, QSlider*>("vLow", vLowSlider));

    QSlider *vHighSlider = new QSlider(Qt::Horizontal);
    setUpSlider(vHighSlider, min, svMax, svMax);
    //vHighSlider->setObjectName("vHighSlider");
    colorSliders.insert(std::pair<std::string, QSlider*>("vHigh", vHighSlider));

    QSlider *erosionIterSlider = new QSlider(Qt::Horizontal);
    setUpSlider(erosionIterSlider, 0, GlobalData::erosionIterMax, GlobalData::erosionIterDefault);
    colorSliders.insert(std::pair<std::string, QSlider *>("erosionIter", erosionIterSlider));

    QSlider *dilationIterSlider = new QSlider(Qt::Horizontal);
    setUpSlider(dilationIterSlider, 0, GlobalData::dilationIterMax, GlobalData::dilationIterDefault);
    colorSliders.insert(std::pair<std::string, QSlider *>("dilationIter", dilationIterSlider));

    QSlider *erosDilaSizeSlider = new QSlider(Qt::Horizontal);
    setUpSlider(erosDilaSizeSlider, 1, GlobalData::erosDilaSizeMax, GlobalData::erosDilaSizeDefault);
    colorSliders.insert(std::pair<std::string, QSlider *>("erosDilaSize", erosDilaSizeSlider));

    // Connect Sliders
    connect(hLowSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSlidersChanged(int)));
    connect(hHighSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSlidersChanged(int)));
    connect(sLowSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSlidersChanged(int)));
    connect(sHighSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSlidersChanged(int)));
    connect(vLowSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSlidersChanged(int)));
    connect(vHighSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSlidersChanged(int)));
    connect(erosionIterSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSlidersChanged(int)));
    connect(dilationIterSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSlidersChanged(int)));
    connect(erosDilaSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSlidersChanged(int)));

    // Add rows to Form Layout
    colorOptionsLayout->addRow(hLowLabel, hLowSlider);
    colorOptionsLayout->addRow(hHighLabel, hHighSlider);
    colorOptionsLayout->addRow(sLowLabel, sLowSlider);
    colorOptionsLayout->addRow(sHighLabel, sHighSlider);
    colorOptionsLayout->addRow(vLowLabel, vLowSlider);
    colorOptionsLayout->addRow(vHighLabel, vHighSlider);
    colorOptionsLayout->addRow(erosionIterLabel, erosionIterSlider);
    colorOptionsLayout->addRow(dilationIterLabel, dilationIterSlider);
    colorOptionsLayout->addRow(erosDilaSizeLabel, erosDilaSizeSlider);

    colorOptionsGroupBox->setLayout(colorOptionsLayout);

    // add to parent layout
    visionOptionsLayout->addWidget(colorOptionsGroupBox);
}

//// Set up the shape options sliders
//void VisionTab::setUpShapeOptions(QVBoxLayout *visionOptionsLayout)
//{
//    QGroupBox *shapeOptionsGroupBox = new QGroupBox(tr("Shape Options"));
//    QFormLayout *shapeOptionsLayout = new QFormLayout();

//    // Labels
//    QLabel *blurSizeLabel = new QLabel(tr("Blur Size:"));
//    QLabel *edgeThreshLabel = new QLabel(tr("Edge Threshold:"));
//    QLabel *polyErrorLabel = new QLabel(tr("Polynomial Error:"));
//    QLabel *frontNumVertLabel = new QLabel(tr("Front # Verticies:"));
//    QLabel *frontMinSizeLabel = new QLabel(tr("Front Min Size:"));
//    QLabel *frontMaxSizeLabel = new QLabel(tr("Front Max Size:"));
//    QLabel *backNumVertLabel = new QLabel(tr("Back # Verticies:"));
//    QLabel *backMinSizeLabel = new QLabel(tr("Back Min Size:"));
//    QLabel *backMaxSizeLabel = new QLabel(tr("Back Max Size:"));

//    // Sliders
//    ShapeData shapeData;
//    QSlider *blurSizeSlider = new QSlider(Qt::Horizontal);
//    setUpSlider(blurSizeSlider, GlobalData::blurSizeMin, 10, 3);
//    shapeSliders.insert(pair<std::string, QSlider *>("blurSize", blurSizeSlider));

//    QSlider *edgeThreshSlider = new QSlider(Qt::Horizontal);
//    setUpSlider(edgeThreshSlider, 0, GlobalData::edgeThreshMax, shapeData.edgeThresh);
//    shapeSliders.insert(pair<std::string, QSlider *>("edgeThresh", edgeThreshSlider));

//    QSlider *polyErrorSlider = new QSlider(Qt::Horizontal);
//    setUpSlider(polyErrorSlider, 0, 10, 3);
//    shapeSliders.insert(pair<std::string, QSlider *>("polyError", polyErrorSlider));

//    QSlider *frontNumVertSlider = new QSlider(Qt::Horizontal);
//    setUpSlider(frontNumVertSlider, 0, 10, 4);
//    shapeSliders.insert(pair<std::string, QSlider *>("frontNumVert", frontNumVertSlider));

//    QSlider *frontMinSizeSlider = new QSlider(Qt::Horizontal);
//    setUpSlider(frontMinSizeSlider, 0, GlobalData::shapeMaxSize, GlobalData::shapeMinSizeDefault);
//    shapeSliders.insert(pair<std::string, QSlider *>("frontMinSize", frontMinSizeSlider));

//    QSlider *frontMaxSizeSlider = new QSlider(Qt::Horizontal);
//    setUpSlider(frontMaxSizeSlider, 0, GlobalData::shapeMaxSize, GlobalData::shapeMaxSizeDefault);
//    shapeSliders.insert(pair<std::string, QSlider *>("frontMaxSize", frontMaxSizeSlider));

//    QSlider *backNumVertSlider = new QSlider(Qt::Horizontal);
//    setUpSlider(backNumVertSlider, 0, 10, 4);
//    shapeSliders.insert(pair<std::string, QSlider *>("backNumVert", backNumVertSlider));

//    QSlider *backMinSizeSlider = new QSlider(Qt::Horizontal);
//    setUpSlider(backMinSizeSlider, 0, GlobalData::shapeMaxSize, GlobalData::shapeMinSizeDefault);
//    shapeSliders.insert(pair<std::string, QSlider *>("backMinSize", backMinSizeSlider));

//    QSlider *backMaxSizeSlider = new QSlider(Qt::Horizontal);
//    setUpSlider(backMaxSizeSlider, 0, GlobalData::shapeMaxSize, GlobalData::shapeMaxSizeDefault);
//    shapeSliders.insert(pair<std::string, QSlider *>("backMaxSize", backMaxSizeSlider));

//    // Connect Sliders
//    connect(blurSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
//    connect(edgeThreshSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
//    connect(polyErrorSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
//    connect(frontNumVertSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
//    connect(frontMinSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
//    connect(frontMaxSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
//    connect(backNumVertSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
//    connect(backMinSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
//    connect(backMaxSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));

//    // Add rows to form layout
//    shapeOptionsLayout->addRow(blurSizeLabel, blurSizeSlider);
//    shapeOptionsLayout->addRow(edgeThreshLabel, edgeThreshSlider);
//    shapeOptionsLayout->addRow(polyErrorLabel, polyErrorSlider);
//    shapeOptionsLayout->addRow(frontNumVertLabel, frontNumVertSlider);
//    shapeOptionsLayout->addRow(frontMinSizeLabel, frontMinSizeSlider);
//    shapeOptionsLayout->addRow(frontMaxSizeLabel, frontMaxSizeSlider);
//    shapeOptionsLayout->addRow(backNumVertLabel, backNumVertSlider);
//    shapeOptionsLayout->addRow(backMinSizeLabel, backMinSizeSlider);
//    shapeOptionsLayout->addRow(backMaxSizeLabel, backMaxSizeSlider);

//    shapeOptionsGroupBox->setLayout(shapeOptionsLayout);

//    // add to parent layout
//    visionOptionsLayout->addWidget(shapeOptionsGroupBox);
//}

// Set up individual sliders
void VisionTab::setUpSlider(QSlider *slider, int min, int max, int val)
{
    slider->setMinimum(min);
    slider->setMaximum(max);
    slider->setValue(val);
    slider->setMinimumWidth(650);
}

// Updates the video with a new frame
void VisionTab::updateVideo(cv::Mat frame)
{
    fitVideo(frame);
    cv::Mat RGBframe;
    QImage img;

    //cv::namedWindow( "Image", CV_WINDOW_AUTOSIZE );

    //cv::imshow( "Image", frame );

    if (frame.channels()== 3){
        cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
        QImage dest((const unsigned char*) RGBframe.data, RGBframe.cols, RGBframe.rows, RGBframe.step, QImage::Format_RGB888);
        dest.bits();
        img = dest;
    }
    else if (frame.channels()== 1)
    {
        cv::cvtColor(frame, RGBframe, CV_GRAY2RGB);
        QImage dest((const unsigned char*) frame.data, frame.cols, frame.rows, frame.step, QImage::Format_Indexed8);
        dest.bits();
        img = dest;
    }
    else
    {
        std::cout << "unknown image format\n";
    }

    if (!img.isNull())
    {
        video->setAlignment(Qt::AlignCenter);
        //video->setPixmap(QPixmap::fromImage(img).scaled(video->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
        video->setPixmap(QPixmap::fromImage(img));
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

//// Return a pointer to the vision data member
//Vision *VisionTab::getVision()
//{
//    return vision;
//}

// Send out all values from color sliders
void VisionTab::colorSlidersChanged(int val)
{
    QList<QGroupBox *> gbList = visionOptionsGroupBox->findChildren<QGroupBox *>();
    QGroupBox *colorOptionsGroupBox = gbList.at(0); // first group box is the color options
    QList<QSlider *> sliderList = colorOptionsGroupBox->findChildren<QSlider *>();

    // pack color data
    ColorData colorData;
    colorData.hLow = sliderList.at(0)->value();
    colorData.hHigh = sliderList.at(1)->value();
    colorData.sLow = sliderList.at(2)->value();
    colorData.sHigh = sliderList.at(3)->value();
    colorData.vLow = sliderList.at(4)->value();
    colorData.vHigh = sliderList.at(5)->value();
    colorData.erosionIter = sliderList.at(6)->value();
    colorData.dilationIter = sliderList.at(7)->value();
    colorData.erosDilaSize = sliderList.at(8)->value();

    emit newColorData(colorData);
}

// Handle use background subtraction change
void VisionTab::useBackSubChanged(bool value)
{
    emit newIsUseBackSub(value);
}

// Handle use color changed
void VisionTab::useColorChanged(bool value)
{
    emit newIsUseColor(value);
}

// Handle use of gray
void VisionTab::useGrayChanged(bool value)
{
    emit newIsUseGray(value);
}

// Retrieve the list of color slider pointers
std::map<std::string, QSlider*> VisionTab::getColorSliders()
{
    return colorSliders;
}

// Retrieve the list of shape slider pointers
std::map<std::string, QSlider*> VisionTab::getShapeSliders()
{
    return shapeSliders;
}
