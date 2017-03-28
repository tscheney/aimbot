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
    connect(this, SIGNAL(newColorData(ColorData)), vision, SLOT(newColorData(ColorData)));
    connect(this, SIGNAL(newShapeData(ShapeData)), vision, SLOT(newShapeData(ShapeData)));
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

    QVBoxLayout *visionOptionsLayout = new QVBoxLayout();

    setUpColorOptions(visionOptionsLayout);
    setUpShapeOptions(visionOptionsLayout);

    visionOptionsGroupBox->setLayout(visionOptionsLayout);
    layout->addWidget(visionOptionsGroupBox);
}

// Set up the color options sliders
void VisionTab::setUpColorOptions(QVBoxLayout *visionOptionsLayout)
{
    QGroupBox *colorOptionsGroupBox = new QGroupBox(tr("Color Options"));
    QFormLayout *colorOptionsLayout = new QFormLayout();

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
    connect(hLowSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSlidersChanged(int)));
    connect(hHighSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSlidersChanged(int)));
    connect(sLowSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSlidersChanged(int)));
    connect(sHighSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSlidersChanged(int)));
    connect(vLowSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSlidersChanged(int)));
    connect(vHighSlider, SIGNAL(valueChanged(int)), this, SLOT(colorSlidersChanged(int)));

    // Add rows to Form Layout
    colorOptionsLayout->addRow(hLowLabel, hLowSlider);
    colorOptionsLayout->addRow(hHighLabel, hHighSlider);
    colorOptionsLayout->addRow(sLowLabel, sLowSlider);
    colorOptionsLayout->addRow(sHighLabel, sHighSlider);
    colorOptionsLayout->addRow(vLowLabel, vLowSlider);
    colorOptionsLayout->addRow(vHighLabel, vHighSlider);

    colorOptionsGroupBox->setLayout(colorOptionsLayout);

    // add to parent layout
    visionOptionsLayout->addWidget(colorOptionsGroupBox);
}

// Set up the shape options sliders
void VisionTab::setUpShapeOptions(QVBoxLayout *visionOptionsLayout)
{
    QGroupBox *shapeOptionsGroupBox = new QGroupBox(tr("Shape Options"));
    QFormLayout *shapeOptionsLayout = new QFormLayout();

    // Labels
    QLabel *blurSizeLabel = new QLabel(tr("Blur Size:"));
    QLabel *edgeThreshLabel = new QLabel(tr("Edge Threshold:"));
    QLabel *polyErrorLabel = new QLabel(tr("Polynomial Error:"));
    QLabel *frontNumVertLabel = new QLabel(tr("Front # Verticies:"));
    QLabel *frontMinSizeLabel = new QLabel(tr("Front Min Size:"));
    QLabel *frontMaxSizeLabel = new QLabel(tr("Front Max Size:"));
    QLabel *backNumVertLabel = new QLabel(tr("Back # Verticies:"));
    QLabel *backMinSizeLabel = new QLabel(tr("Back Min Size:"));
    QLabel *backMaxSizeLabel = new QLabel(tr("Back Max Size:"));

    // Sliders
    ShapeData shapeData;
    QSlider *blurSizeSlider = new QSlider(Qt::Horizontal);
    setUpSlider(blurSizeSlider, GlobalData::blurSizeMin, 10, 3);
    QSlider *edgeThreshSlider = new QSlider(Qt::Horizontal);
    setUpSlider(edgeThreshSlider, 0, GlobalData::edgeThreshMax, shapeData.edgeThresh);
    QSlider *polyErrorSlider = new QSlider(Qt::Horizontal);
    setUpSlider(polyErrorSlider, 0, 10, 3);
    QSlider *frontNumVertSlider = new QSlider(Qt::Horizontal);
    setUpSlider(frontNumVertSlider, 0, 10, 4);
    QSlider *frontMinSizeSlider = new QSlider(Qt::Horizontal);
    setUpSlider(frontMinSizeSlider, 0, GlobalData::shapeMaxSize, GlobalData::shapeMinSizeDefault);
    QSlider *frontMaxSizeSlider = new QSlider(Qt::Horizontal);
    setUpSlider(frontMaxSizeSlider, 0, GlobalData::shapeMaxSize, GlobalData::shapeMaxSizeDefault);
    QSlider *backNumVertSlider = new QSlider(Qt::Horizontal);
    setUpSlider(backNumVertSlider, 0, 10, 4);
    QSlider *backMinSizeSlider = new QSlider(Qt::Horizontal);
    setUpSlider(backMinSizeSlider, 0, GlobalData::shapeMaxSize, GlobalData::shapeMaxSizeDefault);
    QSlider *backMaxSizeSlider = new QSlider(Qt::Horizontal);
    setUpSlider(backMaxSizeSlider, 0, GlobalData::shapeMaxSize, 500);

    // Connect Sliders
    connect(blurSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(edgeThreshSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(polyErrorSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(frontNumVertSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(frontMinSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(frontMaxSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(backNumVertSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(backMinSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(backMaxSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));

    // Add rows to form layout
    shapeOptionsLayout->addRow(blurSizeLabel, blurSizeSlider);
    shapeOptionsLayout->addRow(edgeThreshLabel, edgeThreshSlider);
    shapeOptionsLayout->addRow(polyErrorLabel, polyErrorSlider);
    shapeOptionsLayout->addRow(frontNumVertLabel, frontNumVertSlider);
    shapeOptionsLayout->addRow(frontMinSizeLabel, frontMinSizeSlider);
    shapeOptionsLayout->addRow(frontMaxSizeLabel, frontMaxSizeSlider);
    shapeOptionsLayout->addRow(backNumVertLabel, backNumVertSlider);
    shapeOptionsLayout->addRow(backMinSizeLabel, backMinSizeSlider);
    shapeOptionsLayout->addRow(backMaxSizeLabel, backMaxSizeSlider);

    shapeOptionsGroupBox->setLayout(shapeOptionsLayout);

    // add to parent layout
    visionOptionsLayout->addWidget(shapeOptionsGroupBox);

    // size of blur 0 to 10
    // Edge Threshold 0 to 255
    // polynomial error 0.00 to 0.10
    // front number of verticies 0 to 10
    // front max size 0 to 1000
    // front min size 0 to 1000
    // back number of verticles, min/max size
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

    //cv::namedWindow( "Image", CV_WINDOW_AUTOSIZE );

    //cv::imshow( "Image", frame );

    if (frame.channels()== 3){
        cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
        QImage dest((const unsigned char*) RGBframe.data, RGBframe.cols, RGBframe.rows, RGBframe.step, QImage::Format_RGB888);
        dest.bits();
        img = dest;
    }
    else
    {
        QImage dest((const unsigned char*) frame.data, frame.cols, frame.rows, frame.step, QImage::Format_Indexed8);
        dest.bits();
        img = dest;
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

// Return a pointer to the vision data member
Vision  *VisionTab::getVision()
{
    return vision;
}

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

    emit newColorData(colorData);
}

// Send out all values from shape sliders
void VisionTab::shapeSlidersChanged(int val)
{
    QList<QGroupBox *> gbList = visionOptionsGroupBox->findChildren<QGroupBox *>();
    QGroupBox *shapeOptionsGroupBox = gbList.at(1); // second group box is the shape options
    QList<QSlider *> sliderList = shapeOptionsGroupBox->findChildren<QSlider *>();

    ShapeData shapeData;
    shapeData.blurSize = sliderList.at(0)->value();
    shapeData.edgeThresh = sliderList.at(1)->value();
    shapeData.polyError = double(sliderList.at(2)->value()) / 100;
    shapeData.frontNumVert = sliderList.at(3)->value();
    shapeData.frontMinSize = sliderList.at(4)->value();
    shapeData.frontMaxSize = sliderList.at(5)->value();
    shapeData.backNumVert = sliderList.at(6)->value();
    shapeData.backMinSize = sliderList.at(7)->value();
    shapeData.backMaxSize = sliderList.at(8)->value();

    emit newShapeData(shapeData);
}
