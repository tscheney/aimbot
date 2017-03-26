#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QImage>
#include <QThread>
#include <QVector>
#include <QRect>
#include <QPixmap>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QString>
#include "ui_mainwindow.h"
#include <opencv2/core/core.hpp>
#include "maintab.h"
#include "visiontab.h"
#include "camlistener.h"
#include "vision.h"

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //Display video frame in player UI
    //void updatePlayerUI(cv::Mat frame);
    void insertNewTab(QString name);

signals:
    void sliderChanged(QVector<int> hsv);

private:
    Ui::MainWindow *ui;
    CamListener* camListener;
    QTabWidget *tabs;

};
#endif // MAINWINDOW_H
