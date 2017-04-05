#include "ballvisiontab.h"
#include "moc_ballvisiontab.cpp"

BallVisionTab::BallVisionTab(QWidget *parent, QString name) // : VisionTab(parent, name)
{
    setUpVision(name.toStdString());
    layout = new QVBoxLayout();
    setUpVideo();
    setUpVisionOptions();
    setLayout(layout);
}

BallVisionTab::BallVisionTab(QWidget *parent, QString name, map<string, int> profile) // : VisionTab(parent, name, profile)
{
    setUpVision(name.toStdString());
    layout = new QVBoxLayout();
    setUpVideo();
    setUpVisionOptions();
    setLayout(layout);
    loadProfile(profile);
}

// Set up the vision processing
void BallVisionTab::setUpVision(string name)
{
    vision = new BallVision(0, name);
    connect(vision, SIGNAL(processedImage(cv::Mat)), this, SLOT(updateVideo(cv::Mat)));
    connect(this, SIGNAL(newColorData(ColorData)), vision, SLOT(newColorData(ColorData)));
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
    QLabel *edgeThreshLabel = new QLabel(tr("Edge Threshold:"));
    QLabel *polyErrorLabel = new QLabel(tr("Polynomial Error:"));
    QLabel *minNumVertLabel = new QLabel(tr("Min # Verticies:"));
    QLabel *maxNumVertLabel = new QLabel(tr("Max # Verticies:"));
    QLabel *minSizeLabel = new QLabel(tr("Min Size:"));
    QLabel *maxSizeLabel = new QLabel(tr("Max Size:"));


//    int minNumVert;
//    int maxNumVert;
//    int minSize;
//    int maxSize;

    QLabel *useShapeLabel = new QLabel(tr("Use Shape"));
    QCheckBox *useShapeCheckBox = new QCheckBox();
    useShapeCheckBox->setChecked(true);
    connect(useShapeCheckBox, SIGNAL(toggled(bool)), vision, SLOT(useShape(bool)));
    shapeOptionsLayout->addRow(useShapeLabel, useShapeCheckBox);

    // Sliders
    ShapeData shapeData;
    QSlider *blurSizeSlider = new QSlider(Qt::Horizontal);
    setUpSlider(blurSizeSlider, GlobalData::blurSizeMin, GlobalData::blurSizeMax, GlobalData::blurSizeDefault);
    shapeSliders.insert(pair<std::string, QSlider *>("blurSize", blurSizeSlider));

    QSlider *edgeThreshSlider = new QSlider(Qt::Horizontal);
    setUpSlider(edgeThreshSlider, 0, GlobalData::edgeThreshMax, shapeData.edgeThresh);
    shapeSliders.insert(pair<std::string, QSlider *>("edgeThresh", edgeThreshSlider));

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
    connect(edgeThreshSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(polyErrorSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(minNumVertSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(maxNumVertSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(minSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(maxSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));

    // Add rows to form layout
    shapeOptionsLayout->addRow(blurSizeLabel, blurSizeSlider);
    shapeOptionsLayout->addRow(edgeThreshLabel, edgeThreshSlider);
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
    ballShapeData.blurSize = shapeSliders.at("blurSize")->value();
    ballShapeData.edgeThresh = shapeSliders.at("edgeThresh")->value();
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
