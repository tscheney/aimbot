#include "mainwindow.h"
#include "moc_mainwindow.cpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    // Connect vision threads
    camListener = new CamListener();
    camListener->start(QThread::LowPriority); // starts the thread making run() function run continuousl

    ui->setupUi(this);

    tabs = new QTabWidget(this);
    //mainTab = new QWidget();
    MainTab *mainTab = new MainTab();
    tabs->addTab(mainTab, tr("Main"));
    setCentralWidget(tabs);

    // Establish connections
    connect(mainTab, SIGNAL(addNewTab(QString)), this, SLOT(insertNewTab(QString)));
}

MainWindow::~MainWindow()
{
    delete camListener;
    delete ui;
    QList<QWidget *> tabsList = tabs->findChildren<QWidget *>();
    for(QWidget *tab : tabsList)
    {
    //    delete tab;
    }
    delete tabs;
}

//// Updates the video with a new frame
//void MainWindow::updatePlayerUI(cv::Mat frame)
//{
//    cv::Mat RGBframe;
//    QImage img;
//    if (frame.channels()== 3){
//        cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
//        img = QImage((const unsigned char*)(RGBframe.data),
//                          RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
//    }
//    else
//    {
//        img = QImage((const unsigned char*)(frame.data),
//                             frame.cols,frame.rows,QImage::Format_Indexed8);
//    }

//    if (!img.isNull())
//    {
//        ui->label->setAlignment(Qt::AlignCenter);
//        ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
//    }
//}

void MainWindow::insertNewTab(QString name)
{
    VisionTab *visionTab = new VisionTab(this, name);
    connect(camListener, SIGNAL(rawImage(cv::Mat)), visionTab->getVision(), SLOT(process(cv::Mat)));
    tabs->addTab(visionTab, name);
    tabs->setCurrentIndex(tabs->count() - 1);
}
