#ifndef ROBOTVISIONTAB_H
#define ROBOTVISIONTAB_H

#include "visiontab.h"
#include "robotvision.h"

class RobotVisionTab : public VisionTab
{
    Q_OBJECT
public:
    RobotVisionTab(QWidget *parent, QString name);
    RobotVisionTab(QWidget *parent, QString name, map<string, int> profile);
    RobotVision *getVision();
signals:
    void newShapeData(RobotShapeData shapeData);
public slots:
    void shapeSlidersChanged(int val);
private:
    RobotVision *vision;

    void setUpVision(string name);
    void setUpShapeOptions(QVBoxLayout *visionOptionsLayout);
};

#endif // ROBOTVISIONTAB_H
