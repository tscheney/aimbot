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

    //prefilter = new PreFilter();
    //connect(camListener, SIGNAL(rawImage(cv::Mat)), prefilter, SLOT(rawFrame(cv::Mat)));
    //prefilter->moveToThread(&prefilterThread);
    //prefilterThread.start();

    ui->setupUi(this);
    // Set up tabs
    tabs = new QTabWidget(this);
    MainTab *mainTab = new MainTab(this);
    mainTab->setObjectName("Main");
    tabs->addTab(mainTab, tr("Main"));
    setCentralWidget(tabs);
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    setUpMenuBar();
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

// Set up the menu bar for the main window
void MainWindow::setUpMenuBar()
{
    fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *addVision = new QAction(tr("&Add Vision..."), this);
    addVision->setObjectName("addVision");
    fileMenu->addAction(addVision);
    connect(addVision, SIGNAL(triggered()), this, SLOT(addNewClicked()));

    QAction *saveSettings = new QAction(tr("&Save Settings"), this);
    saveSettings->setObjectName("saveSettings");
    saveSettings->setVisible(false);
    fileMenu->addAction(saveSettings);
    connect(saveSettings, SIGNAL(triggered()), this, SLOT(saveClicked()));

    fileMenu->addSeparator();
    QAction *exit = new QAction(tr("E&xit"), this);
    fileMenu->addAction(exit);
    connect(exit, SIGNAL(triggered()), this, SLOT(close()));
}

//// Updates the video with a new frame
//void MainWindow::updatePlayerUI(cv::Mat frame)
//{Writer
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

// Inserts the given robot vision tab into the tabs
void MainWindow::insertRobotNewTab(RobotVisionTab *robotVisionTab)
{
    connect(camListener, SIGNAL(rawImage(cv::Mat)), robotVisionTab->getVision(), SLOT(process(cv::Mat)));
    //connect(prefilter, SIGNAL(filteredFrame(cv::Mat)), robotVisionTab->getVision(), SLOT(process(cv::Mat)));
    tabs->addTab(robotVisionTab, robotVisionTab->objectName());
    tabs->setCurrentIndex(tabs->count() - 1);
}

// Inserts a new robot tab into the tabs container
void MainWindow::insertNewRobotTab(QString name)
{
    RobotVisionTab *robotVisionTab = new RobotVisionTab(this, name);
    robotVisionTab->setObjectName(name);
    insertRobotNewTab(robotVisionTab);
}

// Inserts a new robot tab into the tabs container with the given profile
void MainWindow::insertNewRobotTab(QString name, map<string, int> profile)
{
    RobotVisionTab *robotVisionTab = new RobotVisionTab(this, name, profile);
    robotVisionTab->setObjectName(name);
    insertRobotNewTab(robotVisionTab);
}

// Inserts the given ball vision tab into the tabs
void MainWindow::insertNewBallTab(BallVisionTab *ballVisionTab)
{
    connect(camListener, SIGNAL(rawImage(cv::Mat)), ballVisionTab->getVision(), SLOT(process(cv::Mat)));
    //connect(prefilter, SIGNAL(filteredFrame(cv::Mat)), ballVisionTab->getVision(), SLOT(process(cv::Mat)));
    tabs->addTab(ballVisionTab, ballVisionTab->objectName());
    tabs->setCurrentIndex(tabs->count() - 1);
}

// Inserts a new ball tab into the tabs container
void MainWindow::insertNewBallTab(QString name)
{
    BallVisionTab *ballVisionTab = new BallVisionTab(this, name);
    ballVisionTab->setObjectName(name);
    insertNewBallTab(ballVisionTab);
}

// Inserts a new tab ball into the tabs container with the given profile
void MainWindow::insertNewBallTab(QString name, map<string, int> profile)
{
    BallVisionTab *ballVisionTab = new BallVisionTab(this, name, profile);
    ballVisionTab->setObjectName(name);
    insertNewBallTab(ballVisionTab);
}

// Hide save settings if on main tab
void MainWindow::tabChanged(int tabIndex)
{
    QAction *saveSettings = findChild<QAction *>("saveSettings");
    if(tabIndex == 0)
    {
        saveSettings->setVisible(false);
    }
    else
    {
        saveSettings->setVisible(true);
    }
}

// Handle add new clicked event
void MainWindow::addNewClicked()
{
    AddNewDialog dialog;
    if(dialog.exec() == QDialog::Accepted)
    {
        Settings settings;
        if(dialog.getName() == "ball") //ball
        {
            if(dialog.getProfile() != GlobalData::newProfileName) // load profile
            {
                map<string, int> profile = settings.getBallProfile(dialog.getProfile());
                insertNewBallTab(dialog.getName(), profile);
            }
            else
            {
                insertNewBallTab(dialog.getName());
            }
        }
        else // robot
        {
            if(dialog.getProfile() != GlobalData::newProfileName) // load profile
            {
                map<string, int> profile = settings.getRobotProfile(dialog.getProfile());
                insertNewRobotTab(dialog.getName(), profile);
            }
            else
            {
                insertNewRobotTab(dialog.getName());
            }
        }
    }
}

// Handle Save settings clicked event
void MainWindow::saveClicked()
{
    QInputDialog dialog(this, Qt::Dialog);
    dialog.setWindowTitle("Save Settings");
    dialog.setLabelText("Enter a name:");
    if(dialog.exec() == QDialog::Accepted)
    {
        VisionTab *visionTab = (VisionTab *) tabs->currentWidget();

        map<string, int> params;

        // Collect params from sliders
        for (auto const& colorSlider : visionTab->getColorSliders())
        {
            params.insert(pair<string, int>(colorSlider.first, colorSlider.second->value()));
        }

        for (auto const& shapeSlider : visionTab->getShapeSliders())
        {
            params.insert(pair<string, int>(shapeSlider.first, shapeSlider.second->value()));
        }

        Settings settings;
        // Write profile to file
        if(visionTab->objectName() == "ball")
        {
            settings.writeBallProfile(dialog.textValue(), params);
        }
        else
        {
            settings.writeRobotProfile(dialog.textValue(), params);
        }

    }
}
