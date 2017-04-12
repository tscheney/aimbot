#ifndef BALLVISIONTAB_H
#define BALLVISIONTAB_H

#include "visiontab.h"
#include "ballvision.h"
#include <QCheckBox>

class BallVisionTab : public VisionTab
{
    Q_OBJECT
public:
    BallVisionTab(QWidget *parent, QString name, bool isHome);
    BallVisionTab(QWidget *parent, QString name, map<string, int> profile, bool isHome);
    BallVision *getVision();
signals:
    void newShapeData(BallShapeData shapeData);
public slots:
    void shapeSlidersChanged(int val);
private:
    BallVision *vision;

    void setUpVision(string name, bool isHome);
    void setUpShapeOptions(QVBoxLayout *visionOptionsLayout);
};

#endif // BALLVISIONTAB_H
