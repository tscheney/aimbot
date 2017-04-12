#include "ballvisiontab.h"
#include "moc_ballvisiontab.cpp"

BallVisionTab::BallVisionTab(QWidget *parent, QString name, bool isHome) // : VisionTab(parent, name)
{
    setUpVision(name.toStdString(), isHome);
    layout = new QVBoxLayout();
    setUpVideo();
    setUpVisionOptions();
    setLayout(layout);
}

BallVisionTab::BallVisionTab(QWidget *parent, QString name, map<string, int> profile, bool isHome) // : VisionTab(parent, name, profile)
{
    setUpVision(name.toStdString(), isHome);
    layout = new QVBoxLayout();
    setUpVideo();
    setUpVisionOptions();
    setLayout(layout);
    loadProfile(profile);
}

// Set up the vision processing
void BallVisionTab::setUpVision(string name, bool isHome)
{
    vision = new BallVision(0, name, isHome);
    connect(vision, SIGNAL(processedImage(cv::Mat)), this, SLOT(updateVideo(cv::Mat)));
    connect(this, SIGNAL(newColorData(ColorData)), vision, SLOT(newColorData(ColorData)));
    connect(this, SIGNAL(newIsUseBackSub(bool)), vision, SLOT(useBackSub(bool)));
    connect(this, SIGNAL(newIsUseColor(bool)), vision, SLOT(useColor(bool)));
    connect(this, SIGNAL(newShapeData(BallShapeData)), vision, SLOT(newShapeData(BallShapeData)));
    vision->moveToThread(&visionThread);
    visionThread.start();
}

// Set up the shape options sliders
void BallVisionTab::setUpShapeOptions(QVBoxLayout *visionOptionsLayout)
{
    QGroupBox *shapeOptionsGroupBox = new QGroupBox(tr("Shape Options"));
    QFormLayout *shapeOptionsLayout = new QFormLayout();

    // Labels
    QLabel *blurSizeLabel = new QLabel(tr("Blur Size:"));
    QLabel *edgeThresh1Label = new QLabel(tr("Edge Threshold 1:"));
    QLabel *edgeThresh2Label = new QLabel(tr("Edge Threshold 2:"));
    QLabel *dilationIterLabel = new QLabel(tr("Dilate Iterations:"));
    QLabel *polyErrorLabel = new QLabel(tr("Polynomial Error:"));
    QLabel *minNumVertLabel = new QLabel(tr("Min # Verticies:"));
    QLabel *maxNumVertLabel = new QLabel(tr("Max # Verticies:"));
    QLabel *minSizeLabel = new QLabel(tr("Min Size:"));
    QLabel *maxSizeLabel = new QLabel(tr("Max Size:"));


//    int minNumVert;
//    int maxNumVert;
//    int minSize;
//    int maxSize;
    QLabel *useEdgeDetectLabel = new QLabel(tr("Use Edge Detect"));
    QCheckBox *useEdgeDetectCheckBox = new QCheckBox();
    useEdgeDetectCheckBox->setChecked(true);
    connect(useEdgeDetectCheckBox, SIGNAL(toggled(bool)), vision, SLOT(useEdgeDetect(bool)));

    // Sliders
    ShapeData shapeData;
    QSlider *blurSizeSlider = new QSlider(Qt::Horizontal);
    setUpSlider(blurSizeSlider, GlobalData::blurSizeMin, GlobalData::blurSizeMax, GlobalData::blurSizeDefault);
    shapeSliders.insert(pair<std::string, QSlider *>("blurSize", blurSizeSlider));

    QSlider *edgeThresh1Slider = new QSlider(Qt::Horizontal);
    setUpSlider(edgeThresh1Slider, 0, GlobalData::edgeThreshMax, shapeData.edgeThresh1);
    shapeSliders.insert(pair<std::string, QSlider *>("edgeThresh1", edgeThresh1Slider));

    QSlider *edgeThresh2Slider = new QSlider(Qt::Horizontal);
    setUpSlider(edgeThresh2Slider, 0, GlobalData::edgeThreshMax, shapeData.edgeThresh2);
    shapeSliders.insert(pair<std::string, QSlider *>("edgeThresh2", edgeThresh2Slider));

    QSlider *dilationIterSlider = new QSlider(Qt::Horizontal);
    setUpSlider(dilationIterSlider, 0, GlobalData::dilationIterMax, shapeData.dilationIter);
    shapeSliders.insert(pair<std::string, QSlider *>("dilationIter", dilationIterSlider));

    QLabel *useShapeLabel = new QLabel(tr("Use Shape"));
    QCheckBox *useShapeCheckBox = new QCheckBox();
    useShapeCheckBox->setChecked(true);
    connect(useShapeCheckBox, SIGNAL(toggled(bool)), vision, SLOT(useShape(bool)));

    QSlider *polyErrorSlider = new QSlider(Qt::Horizontal);
    int polyErrorSliderMax = int(GlobalData::polyErrorMax * GlobalData::polyErrorSliderDivisor);
    int polyErrorSliderDefault = int(GlobalData::polyErrorDefault * GlobalData::polyErrorSliderDivisor);
    setUpSlider(polyErrorSlider, 0, polyErrorSliderMax, polyErrorSliderDefault);
    shapeSliders.insert(pair<std::string, QSlider *>("polyError", polyErrorSlider));

    QSlider *minNumVertSlider = new QSlider(Qt::Horizontal);
    setUpSlider(minNumVertSlider, 0, GlobalData::shapeMaxNumVert, GlobalData::shapeMinNumVertDefault);
    shapeSliders.insert(pair<std::string, QSlider *>("minNumVert", minNumVertSlider));

    QSlider *maxNumVertSlider = new QSlider(Qt::Horizontal);
    setUpSlider(maxNumVertSlider, 0, GlobalData::shapeMaxNumVert, GlobalData::shapeMaxNumVertDefault);
    shapeSliders.insert(pair<std::string, QSlider *>("maxNumVert", maxNumVertSlider));

    QSlider *minSizeSlider = new QSlider(Qt::Horizontal);
    setUpSlider(minSizeSlider, 0, GlobalData::shapeMaxSize, GlobalData::shapeMinSizeDefault);
    shapeSliders.insert(pair<std::string, QSlider *>("minSize", minSizeSlider));

    QSlider *maxSizeSlider = new QSlider(Qt::Horizontal);
    setUpSlider(maxSizeSlider, 0, GlobalData::shapeMaxSize, GlobalData::shapeMaxSizeDefault);
    shapeSliders.insert(pair<std::string, QSlider *>("maxSize", maxSizeSlider));

    // Connect Sliders
    connect(blurSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(edgeThresh1Slider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(edgeThresh2Slider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(dilationIterSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(polyErrorSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(minNumVertSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(maxNumVertSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(minSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(maxSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));

    // Add rows to form layout
    shapeOptionsLayout->addRow(useEdgeDetectLabel, useEdgeDetectCheckBox);
    shapeOptionsLayout->addRow(blurSizeLabel, blurSizeSlider);
    shapeOptionsLayout->addRow(edgeThresh1Label, edgeThresh1Slider);
    shapeOptionsLayout->addRow(edgeThresh2Label, edgeThresh2Slider);
    shapeOptionsLayout->addRow(useShapeLabel, useShapeCheckBox);
    shapeOptionsLayout->addRow(dilationIterLabel, dilationIterSlider);
    shapeOptionsLayout->addRow(polyErrorLabel, polyErrorSlider);
    shapeOptionsLayout->addRow(minNumVertLabel, minNumVertSlider);
    shapeOptionsLayout->addRow(maxNumVertLabel, maxNumVertSlider);
    shapeOptionsLayout->addRow(minSizeLabel, minSizeSlider);
    shapeOptionsLayout->addRow(maxSizeLabel, maxSizeSlider);

    shapeOptionsGroupBox->setLayout(shapeOptionsLayout);

    // add to parent layout
    visionOptionsLayout->addWidget(shapeOptionsGroupBox);
}

// Send out all values from shape sliders
void BallVisionTab::shapeSlidersChanged(int val)
{
    BallShapeData ballShapeData;
    ballShapeData.blurSize = ballShapeData.edgeThSlidToNum(shapeSliders.at("blurSize")->value());
    ballShapeData.edgeThresh1 = shapeSliders.at("edgeThresh1")->value();
    ballShapeData.edgeThresh2 = shapeSliders.at("edgeThresh2")->value();
    ballShapeData.dilationIter = shapeSliders.at("dilationIter")->value();
    ballShapeData.polyError = double(shapeSliders.at("polyError")->value()) / GlobalData::polyErrorSliderDivisor;
    ballShapeData.minNumVert = shapeSliders.at("minNumVert")->value();
    ballShapeData.maxNumVert = shapeSliders.at("maxNumVert")->value();
    ballShapeData.minSize = shapeSliders.at("minSize")->value();
    ballShapeData.maxSize = shapeSliders.at("maxSize")->value();

    emit newShapeData(ballShapeData);
}

// Return a pointer to the vision data member
BallVision *BallVisionTab::getVision()
{
    return vision;
}
