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
extern const std::string spacePrefix;

extern const int colorHMax;
extern const int colorSVMax;

extern const int blurSizeMin;
extern const int blurSizeMax;
extern const int blurSizeDefault;
extern const int edgeThreshMax;
extern const int erosionIterDefault;
extern const int erosionIterMax;
extern const int dilationIterDefault;
extern const int dilationIterMax;
extern const int erosDilaSizeDefault;
extern const int erosDilaSizeMax;
extern const int glareThreshMax;
extern const double polyErrorMax;
extern const double polyErrorDefault;
extern const int polyErrorSliderDivisor;
extern const int shapeMaxSize;
extern const int shapeMinSizeDefault;
extern const int shapeMaxSizeDefault;
extern const int shapeMaxNumVert;
extern const int shapeMinNumVertDefault;
extern const int shapeMaxNumVertDefault;
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
    int erosionIter;
    int dilationIter;
    int erosDilaSize;

    ColorData(int inhLow = 0, int inhHigh = 179, int insLow = 0, int insHigh = 255, int invLow = 0, int invHigh = 255,
              int inErosionIter = GlobalData::erosionIterDefault, int inDilationIter = GlobalData::dilationIterDefault,
              int inErosDilaSize = GlobalData::erosDilaSizeDefault);
};

class ShapeData
{
public:
    int blurSize;
    int edgeThresh1;
    int edgeThresh2;
    int dilationIter;
    double polyError;

    ShapeData(int inBlurSize = 3, int inEdgeThresh1 = 100, int inEdgeThresh2 = 200, int inDilationIter = GlobalData::dilationIterDefault, double inPolyError = 0.03);

    virtual std::string toString();

    int edgeThSlidToNum(int slideValue);
};

class RobotShapeData : public ShapeData
{
public:
    int frontMinNumVert;
    int frontMaxNumVert;
    int frontMinSize;
    int frontMaxSize;
    int backMinNumVert;
    int backMaxNumVert;
    int backMinSize;
    int backMaxSize;

    RobotShapeData(int inBlurSize = 3, int inEdgeThresh = 100, int inEdgeThresh2 = 200, int inDilationIter = GlobalData::dilationIterDefault, double inPolyError = 0.03, int inFrontMinNumVert = 3, int inFrontMaxNumVert =5, int inFrontMinSize = GlobalData::shapeMinSizeDefault,
                   int inFrontMaxSize = GlobalData::shapeMaxSizeDefault, int inBackMinNumVert = 3, int inBackMaxNumvert = 5, int inBackMinSize = GlobalData::shapeMinSizeDefault,
                   int inBackMaxSize = GlobalData::shapeMaxSizeDefault);

    std::string toString();
};

class BallShapeData : public ShapeData
{
public:
    int minNumVert;
    int maxNumVert;
    int minSize;
    int maxSize;

    BallShapeData(int inBlurSize = 3, int inEdgeThresh1 = 100, int inEdgeThresh2 = 200, int inDilationIter = GlobalData::dilationIterDefault, double inPolyError = 0.03, int inMinNumVert = 6, int inMaxNumVert = 50, int inMinSize = GlobalData::shapeMinSizeDefault,
                  int inMaxSize = GlobalData::shapeMaxSizeDefault);

    std::string toString();
};

#endif // GLOBAL_H
