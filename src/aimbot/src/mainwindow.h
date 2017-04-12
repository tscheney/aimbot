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
#include <QMenuBar>
#include <QSettings>
#include <QtXml>
#include "ui_mainwindow.h"
#include <opencv2/core/core.hpp>
#include "maintab.h"
#include "visiontab.h"
#include "robotvisiontab.h"
#include "ballvisiontab.h"
#include "camlistener.h"
#include "vision.h"
#include "settings.h"
#include "prefilter.h"

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    //Display video frame in player UI
    //void updatePlayerUI(cv::Mat frame);
    void tabChanged(int tabIndex);
    void addNewClicked();
    void saveClicked();
    void loadClicked();
    void runVision(bool isRun);
    void teamSideChanged(bool inIsHome);
    void useBackSub(bool isCalc);

signals:
    void sliderChanged(QVector<int> hsv);
    void runListener(bool isRun);
    void newTeamSide(bool outIsHome);
    void reinitPublishers();
    void newUseBackSub(bool isCalc);
private:
    Ui::MainWindow *ui;
    QMenu *fileMenu;
    CamListener* camListener;
    QTabWidget *tabs;
    PreFilter *prefilter;
    QThread prefilterThread;
    bool isHome = true;

    void setUpMenuBar();
    void insertRobotNewTab(RobotVisionTab *robotVisionTab);
    void insertNewRobotTab(QString name);
    void insertNewRobotTab(QString name, map<string, int> profile);
    void insertNewBallTab(BallVisionTab *ballVisionTab);
    void insertNewBallTab(QString name);
    void insertNewBallTab(QString name, map<string, int> profile);

};
#endif // MAINWINDOW_H
