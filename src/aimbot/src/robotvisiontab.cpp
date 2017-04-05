#include "robotvisiontab.h"
#include "moc_robotvisiontab.cpp"

RobotVisionTab::RobotVisionTab(QWidget *parent, QString name) // : VisionTab(parent, name)
{
    setUpVision(name.toStdString());
    layout = new QVBoxLayout();
    setUpVideo();
    setUpVisionOptions();
    setLayout(layout);
}

RobotVisionTab::RobotVisionTab(QWidget *parent, QString name, map<string, int> profile) // : VisionTab(parent, name, profile)
{
    setUpVision(name.toStdString());
    layout = new QVBoxLayout();
    setUpVideo();
    setUpVisionOptions();
    setLayout(layout);
    loadProfile(profile);
}

// Set up the vision processing
void RobotVisionTab::setUpVision(string name)
{
    vision = new RobotVision(0, name);
    connect(vision, SIGNAL(processedImage(cv::Mat)), this, SLOT(updateVideo(cv::Mat)));
    connect(this, SIGNAL(newColorData(ColorData)), vision, SLOT(newColorData(ColorData)));
    connect(this, SIGNAL(newShapeData(RobotShapeData)), vision, SLOT(newShapeData(RobotShapeData)));
    vision->moveToThread(&visionThread);
    visionThread.start();
}

// Set up the shape options sliders
void RobotVisionTab::setUpShapeOptions(QVBoxLayout *visionOptionsLayout)
{
    QGroupBox *shapeOptionsGroupBox = new QGroupBox(tr("Shape Options"));
    QFormLayout *shapeOptionsLayout = new QFormLayout();

    // Labels
    QLabel *blurSizeLabel = new QLabel(tr("Blur Size:"));
    QLabel *edgeThreshLabel = new QLabel(tr("Edge Threshold:"));
    QLabel *polyErrorLabel = new QLabel(tr("Polynomial Error:"));
    QLabel *frontMinNumVertLabel = new QLabel(tr("Front Min # Verticies:"));
    QLabel *frontMaxNumVertLabel = new QLabel(tr("Front Max # Verticies:"));
    QLabel *frontMinSizeLabel = new QLabel(tr("Front Min Size:"));
    QLabel *frontMaxSizeLabel = new QLabel(tr("Front Max Size:"));
    QLabel *backMinNumVertLabel = new QLabel(tr("Back Min # Verticies:"));
    QLabel *backMaxNumVertLabel = new QLabel(tr("Back Max # Verticies:"));
    QLabel *backMinSizeLabel = new QLabel(tr("Back Min Size:"));
    QLabel *backMaxSizeLabel = new QLabel(tr("Back Max Size:"));

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

    QSlider *frontMinNumVertSlider = new QSlider(Qt::Horizontal);
    setUpSlider(frontMinNumVertSlider, 0, GlobalData::shapeMaxNumVert, GlobalData::shapeMinNumVertDefault);
    shapeSliders.insert(pair<std::string, QSlider *>("frontMinNumVert", frontMinNumVertSlider));

    QSlider *frontMaxNumVertSlider = new QSlider(Qt::Horizontal);
    setUpSlider(frontMaxNumVertSlider, 0, GlobalData::shapeMaxNumVert, GlobalData::shapeMaxNumVertDefault);
    shapeSliders.insert(pair<std::string, QSlider *>("frontMaxNumVert", frontMaxNumVertSlider));

    QSlider *frontMinSizeSlider = new QSlider(Qt::Horizontal);
    setUpSlider(frontMinSizeSlider, 0, GlobalData::shapeMaxSize, GlobalData::shapeMinSizeDefault);
    shapeSliders.insert(pair<std::string, QSlider *>("frontMinSize", frontMinSizeSlider));

    QSlider *frontMaxSizeSlider = new QSlider(Qt::Horizontal);
    setUpSlider(frontMaxSizeSlider, 0, GlobalData::shapeMaxSize, GlobalData::shapeMaxSizeDefault);
    shapeSliders.insert(pair<std::string, QSlider *>("frontMaxSize", frontMaxSizeSlider));

    QSlider *backMinNumVertSlider = new QSlider(Qt::Horizontal);
    setUpSlider(backMinNumVertSlider, 0, GlobalData::shapeMaxNumVert, GlobalData::shapeMinNumVertDefault);
    shapeSliders.insert(pair<std::string, QSlider *>("backMinNumVert", backMinNumVertSlider));

    QSlider *backMaxNumVertSlider = new QSlider(Qt::Horizontal);
    setUpSlider(backMaxNumVertSlider, 0, GlobalData::shapeMaxNumVert, GlobalData::shapeMaxNumVertDefault);
    shapeSliders.insert(pair<std::string, QSlider *>("backMaxNumVert", backMaxNumVertSlider));

    QSlider *backMinSizeSlider = new QSlider(Qt::Horizontal);
    setUpSlider(backMinSizeSlider, 0, GlobalData::shapeMaxSize, GlobalData::shapeMinSizeDefault);
    shapeSliders.insert(pair<std::string, QSlider *>("backMinSize", backMinSizeSlider));

    QSlider *backMaxSizeSlider = new QSlider(Qt::Horizontal);
    setUpSlider(backMaxSizeSlider, 0, GlobalData::shapeMaxSize, GlobalData::shapeMaxSizeDefault);
    shapeSliders.insert(pair<std::string, QSlider *>("backMaxSize", backMaxSizeSlider));

    // Connect Sliders
    connect(blurSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(edgeThreshSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(polyErrorSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(frontMinNumVertSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(frontMaxNumVertSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(frontMinSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(frontMaxSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(backMinNumVertSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(backMaxNumVertSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(backMinSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));
    connect(backMaxSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(shapeSlidersChanged(int)));

    // Add rows to form layout
    shapeOptionsLayout->addRow(blurSizeLabel, blurSizeSlider);
    shapeOptionsLayout->addRow(edgeThreshLabel, edgeThreshSlider);
    shapeOptionsLayout->addRow(polyErrorLabel, polyErrorSlider);
    shapeOptionsLayout->addRow(frontMinNumVertLabel, frontMinNumVertSlider);
    shapeOptionsLayout->addRow(frontMaxNumVertLabel, frontMaxNumVertSlider);
    shapeOptionsLayout->addRow(frontMinSizeLabel, frontMinSizeSlider);
    shapeOptionsLayout->addRow(frontMaxSizeLabel, frontMaxSizeSlider);
    shapeOptionsLayout->addRow(backMinNumVertLabel, backMinNumVertSlider);
    shapeOptionsLayout->addRow(backMaxNumVertLabel, backMaxNumVertSlider);
    shapeOptionsLayout->addRow(backMinSizeLabel, backMinSizeSlider);
    shapeOptionsLayout->addRow(backMaxSizeLabel, backMaxSizeSlider);

    shapeOptionsGroupBox->setLayout(shapeOptionsLayout);

    // add to parent layout
    visionOptionsLayout->addWidget(shapeOptionsGroupBox);
}

// Send out all values from shape sliders
void RobotVisionTab::shapeSlidersChanged(int val)
{
    RobotShapeData robotShapeData;
    robotShapeData.blurSize = shapeSliders.at("blurSize")->value();
    robotShapeData.edgeThresh = shapeSliders.at("edgeThresh")->value();
    robotShapeData.polyError = double(shapeSliders.at("polyError")->value()) / GlobalData::polyErrorSliderDivisor;
    robotShapeData.frontMinNumVert = shapeSliders.at("frontMinNumVert")->value();
    robotShapeData.frontMaxNumVert = shapeSliders.at("frontMaxNumVert")->value();
    robotShapeData.frontMinSize = shapeSliders.at("frontMinSize")->value();
    robotShapeData.frontMaxSize = shapeSliders.at("frontMaxSize")->value();
    robotShapeData.backMinNumVert = shapeSliders.at("backMinNumVert")->value();
    robotShapeData.backMaxNumVert = shapeSliders.at("backMaxNumVert")->value();
    robotShapeData.backMinSize = shapeSliders.at("backMinSize")->value();
    robotShapeData.backMaxSize = shapeSliders.at("backMaxSize")->value();

    emit newShapeData(robotShapeData);
}

// Return a pointer to the vision data member
RobotVision *RobotVisionTab::getVision()
{
    return vision;
}


