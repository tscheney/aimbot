#ifndef MAINTAB_H
#define MAINTAB_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QString>
#include "mainwindow.h"
#include "addnewdialog.h"

class MainTab : public QWidget
{
    Q_OBJECT
public:
    explicit MainTab(QWidget *parent = 0);
    ~MainTab();

private:
    QGridLayout *layout;
    QPushButton *addNewVisionButton;
};

#endif // MAINTAB_H
