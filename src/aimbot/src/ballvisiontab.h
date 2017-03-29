#ifndef BALLVISIONTAB_H
#define BALLVISIONTAB_H

#include "visiontab.h"
#include "ballvision.h"

class BallVisionTab : public VisionTab
{
    Q_OBJECT
public:
    BallVisionTab(QWidget *parent, QString name);
    BallVisionTab(QWidget *parent, QString name, map<string, int> profile);
    BallVision *getVision();
signals:
    void newShapeData(BallShapeData shapeData);
public slots:
    void shapeSlidersChanged(int val);
private:
    BallVision *vision;

    void setUpVision(string name);
    void setUpShapeOptions(QVBoxLayout *visionOptionsLayout);
};

#endif // BALLVISIONTAB_H
