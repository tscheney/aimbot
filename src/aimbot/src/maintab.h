#ifndef MAINTAB_H
#define MAINTAB_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QString>
#include <QRadioButton>
#include "mainwindow.h"
#include "addnewdialog.h"

class MainTab : public QWidget
{
    Q_OBJECT
public:
    explicit MainTab(QWidget *parent = 0);
    ~MainTab();

signals:
    void changeRosState(bool isRun);
    void teamSideChanged(bool isHome);
public slots:
    void startRosNodesClicked();
    void rosNodesRunning(bool value);
    void homeCheckedChanged(bool value);
    void rosNodesOutput(std::string text);
private:
    QGridLayout *layout;
    QPushButton *addNewVisionButton;
    QPushButton *startRosNodesButton;
    QLabel *outputLabel;
    QRadioButton *homeRadioButton;
    QRadioButton *awayRadioButton;
    bool isRosRunning;

    void setUpRosNode();

};

#endif // MAINTAB_H
