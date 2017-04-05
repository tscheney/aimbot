#include "global.h"
#include "moc_global.cpp"


namespace GlobalData
{
    const QString settingsPath = QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/.config/aimbot/";
    const QString profilesFile = "profiles.xml";
    const QString robotGroupName = "robots";
    const QString ballGroupName = "balls";
    extern const QString newProfileName = "New";

    const int blurSizeMin = 1;
    const int blurSizeMax = 50;
    const int blurSizeDefault = 3;
    const int edgeThreshMax = 255;
    const int glareThreshMax = 255;
    const double polyErrorMax = 0.2;
    const double polyErrorDefault = 0.03;
    const int polyErrorSliderDivisor = 1000;
    const int shapeMaxSize = 5000;
    const int shapeMinSizeDefault = 100;
    const int shapeMaxSizeDefault = 2000;
    const int shapeMaxNumVert = 100;
    const int shapeMinNumVertDefault = 10;
    const int shapeMaxNumVertDefault = 50;
}

ColorData::ColorData(int inhLow, int inhHigh, int insLow, int insHigh, int invLow, int invHigh)
{
    hLow = inhLow;
    hHigh = inhHigh;
    sLow = insLow;
    sHigh = insHigh;
    vLow = invLow;
    vHigh = invHigh;
}

ShapeData::ShapeData(int inBlurSize, int inEdgeThresh, double inPolyError)
{
    blurSize = inBlurSize;
    edgeThresh = inEdgeThresh;
    polyError = inPolyError;
}

std::string ShapeData::toString()
{
    std::string out = "blur size: " + std::to_string(blurSize) + "\n";
    out += "edge threshold: " + std::to_string(edgeThresh) + "\n";
    out += "polynomial error: " + std::to_string(polyError) + "\n";

    return out;
}

RobotShapeData::RobotShapeData(int inBlurSize, int inEdgeThresh, double inPolyError, int inFrontMinNumVert, int inFrontMaxNumVert, int inFrontMinSize, int inFrontMaxSize,
                     int inBackMinNumVert, int inBackMaxNumVert, int inBackMinSize, int inBackMaxSize)
{
    blurSize = inBlurSize;
    edgeThresh = inEdgeThresh;
    polyError = inPolyError;
    frontMinNumVert = inFrontMinNumVert;
    frontMaxNumVert = inFrontMaxNumVert;
    frontMinSize = inFrontMinSize;
    frontMaxSize = inFrontMaxSize;
    backMinNumVert = inBackMinNumVert;
    backMaxNumVert = inBackMaxNumVert;
    backMinSize = inBackMinSize;
    backMaxSize = inBackMaxSize;
}

std::string RobotShapeData::toString()
{
    std::string out = "blur size: " + std::to_string(blurSize) + "\n";
    out += "edge threshold: " + std::to_string(edgeThresh) + "\n";
    out += "polynomial error: " + std::to_string(polyError) + "\n";
    out += "front min num verticies: " + std::to_string(frontMinNumVert) + "\n";
    out += "front max num verticies: " + std::to_string(frontMaxNumVert) + "\n";
    out += "front min size: " + std::to_string(frontMinSize)  + "\n";
    out += "front max size: " + std::to_string(frontMaxSize) + "\n";
    out += "back min num verticies: " + std::to_string(backMinNumVert) + "\n";
    out += "back max num verticies: " + std::to_string(backMaxNumVert) + "\n";
    out += "back min size: " + std::to_string(backMinSize)  + "\n";
    out += "back max size: " + std::to_string(backMaxSize) + "\n";

    return out;
}

BallShapeData::BallShapeData(int inBlurSize, int inEdgeThresh, double inPolyError, int inMinNumVert, int inMaxNumVert, int inMinSize, int inMaxSize)
{
    blurSize = inBlurSize;
    edgeThresh = inEdgeThresh;
    polyError = inPolyError;
    minNumVert = inMinNumVert;
    maxNumVert = inMaxNumVert;
    minSize = inMinSize;
    maxSize = inMaxSize;
}

std::string BallShapeData::toString()
{
    std::string out = "blur size: " + std::to_string(blurSize) + "\n";
    out += "edge threshold: " + std::to_string(edgeThresh) + "\n";
    out += "polynomial error: " + std::to_string(polyError) + "\n";
    out += "min num vert: " + std::to_string(minNumVert) + "\n";
    out += "max num vert: " + std::to_string(maxNumVert) + "\n";
    out += "min size: " + std::to_string(minSize) + "\n";
    out += "max size: " + std::to_string(maxSize) + "\n";

    return out;
}
