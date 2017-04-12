#ifndef MAINTAB_H
#define MAINTAB_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QString>
#include <QRadioButton>
#include <QCheckBox>
#include "mainwindow.h"
#include "addnewdialog.h"

class MainTab : public QWidget
{
    Q_OBJECT
public:
    explicit MainTab(QWidget *parent = 0);
    ~MainTab();
public slots:
    void homeCheckedChanged(bool value);
    void startStopButtonPushed();
signals:
    void runVision(bool isRun);
    void teamSideChanged(bool isHome);
private:
    QGridLayout *layout;
    QPushButton *addNewVisionButton;
    QLabel *outputLabel;
    QRadioButton *homeRadioButton;
    QRadioButton *awayRadioButton;
    QPushButton *startStopButton;
    QCheckBox *backSubCheckBox;
    bool isStarted;

    void setUpTeamSide();
    void setUpPreFilter();
};

#endif // MAINTAB_H
