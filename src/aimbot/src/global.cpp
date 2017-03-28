#include "global.h"


namespace GlobalData
{
    const int edgeThreshMax = 255;
    const int blurSizeMin = 1;
    const int shapeMaxSize = 50000;
    const int shapeMinSizeDefault = 100;
    const int shapeMaxSizeDefault = 20000;
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


ShapeData::ShapeData(int inBlurSize, int inEdgeThresh, double inPolyError, int inFrontNumVert, int inFrontMinSize, int inFrontMaxSize,
                     int inBackNumVert, int inBackMinSize, int inBackMaxSize)
{
    blurSize = inBlurSize;
    edgeThresh = inEdgeThresh;
    polyError = inPolyError;
    frontNumVert = inFrontNumVert;
    frontMinSize = inFrontMinSize;
    frontMaxSize = inFrontMaxSize;
    backNumVert = inBackNumVert;
    backMinSize = inBackMinSize;
    backMaxSize = inBackMaxSize;
}

std::string ShapeData::toString()
{
    std::string out = "blur size: " + std::to_string(blurSize) + "\n";
    out += "edge threshold: " + std::to_string(edgeThresh) + "\n";
    out += "polynomial error: " + std::to_string(polyError) + "\n";
    out += "front num verticies: " + std::to_string(frontNumVert) + "\n";
    out += "front min size: " + std::to_string(frontMinSize)  + "\n";
    out += "front max size: " + std::to_string(frontMaxSize) + "\n";
    out += "back num verticies: " + std::to_string(backNumVert) + "\n";
    out += "back min size: " + std::to_string(backMinSize)  + "\n";
    out += "back max size: " + std::to_string(backMaxSize) + "\n";

    return out;
}
