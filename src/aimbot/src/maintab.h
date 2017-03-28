#ifndef MAINTAB_H
#define MAINTAB_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QString>
#include "mainwindow.h"

class MainTab : public QWidget
{
    Q_OBJECT
public:
    explicit MainTab(QWidget *parent = 0);
    ~MainTab();

signals:
    void addNewTab(QString name);
public slots:
    //
    void addNewClicked();

private:
    QGridLayout *layout;
    QPushButton *addNewVision;
};

#endif // MAINTAB_H
