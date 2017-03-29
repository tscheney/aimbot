#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QDesktopServices>
#include <string>


namespace GlobalData
{
    extern const QString settingsPath;
    extern const QString profilesFile;
    extern const QString robotGroupName;
    extern const QString ballGroupName;
    extern const QString newProfileName;
    extern const int edgeThreshMax;
    extern const int blurSizeMin;
    extern const int shapeMaxSize;
    extern const int shapeMinSizeDefault;
    extern const int shapeMaxSizeDefault;
}

class ColorData
{
public:
    int hLow;
    int hHigh;
    int sLow;
    int sHigh;
    int vLow;
    int vHigh;

    ColorData(int inhLow = 0, int inhHigh = 179, int insLow = 0, int insHigh = 255, int invLow = 0, int invHigh = 255);
};

class ShapeData
{
public:
    int blurSize;
    int edgeThresh;
    double polyError;
    int frontNumVert;
    int frontMinSize;
    int frontMaxSize;
    int backNumVert;
    int backMinSize;
    int backMaxSize;

    ShapeData(int inBlurSize = 3, int inEdgeThresh = 100, double inPolyError = 0.03, int inFrontNumVert = 4, int inFrontMinSize = 100, int inFrontMaxSize = 500,
            int inBackNumVert = 4, int inBackMinSize = 100, int inBackMaxSize = 500);

    std::string toString();
};

#endif // GLOBAL_H
