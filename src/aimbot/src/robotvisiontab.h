#ifndef ROBOTVISIONTAB_H
#define ROBOTVISIONTAB_H

#include <QCheckBox>
#include "visiontab.h"
#include "robotvision.h"

class RobotVisionTab : public VisionTab
{
    Q_OBJECT
public:
    RobotVisionTab(QWidget *parent, QString name, bool isHome);
    RobotVisionTab(QWidget *parent, QString name, map<string, int> profile, bool isHome);
    RobotVision *getVision();
signals:
    void newShapeData(RobotShapeData shapeData);
public slots:
    void shapeSlidersChanged(int val);
private:
    RobotVision *vision;

    void setUpVision(string name, bool isHome);
    void setUpShapeOptions(QVBoxLayout *visionOptionsLayout);
};

#endif // ROBOTVISIONTAB_H
