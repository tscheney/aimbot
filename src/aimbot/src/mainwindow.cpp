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

// Inserts the given vision tab into the tabs
void MainWindow::insertNewTab(VisionTab *visionTab)
{
    connect(camListener, SIGNAL(rawImage(cv::Mat)), visionTab->getVision(), SLOT(process(cv::Mat)));
    tabs->addTab(visionTab, visionTab->objectName());
    tabs->setCurrentIndex(tabs->count() - 1);
}

// Inserts a new tab into the tabs container
void MainWindow::insertNewTab(QString name)
{
    VisionTab *visionTab = new VisionTab(this, name);
    visionTab->setObjectName(name);
    insertNewTab(visionTab);
}

// Inserts a new tab into the tabs container with the given profile
void MainWindow::insertNewTab(QString name, map<string, int> profile)
{
    VisionTab *visionTab = new VisionTab(this, name, profile);
    visionTab->setObjectName(name);
    insertNewTab(visionTab);
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
        if(dialog.getProfile() != GlobalData::newProfileName)
        {
            Settings settings;
            map<string, int> profile = settings.getRobotProfile(dialog.getProfile());
            printf("hLow %d\n\r", profile.at("hLow"));
            printf("edgeThresh %d\n\r", profile.at("edgeThresh"));
            insertNewTab(dialog.getName(), profile);
        }
        else
        {
            insertNewTab(dialog.getName());
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

        // get color params
        int hLow = visionTab->getColorSliders().at("hLow")->value();
        params.insert(pair<string, int>("hLow", hLow));
        int hHigh = visionTab->getColorSliders().at("hHigh")->value();
        params.insert(pair<string, int>("hHigh", hHigh));
        int sLow = visionTab->getColorSliders().at("sLow")->value();
        params.insert(pair<string, int>("sLow", sLow));
        int sHigh = visionTab->getColorSliders().at("sHigh")->value();
        params.insert(pair<string, int>("sHigh", sHigh));
        int vLow = visionTab->getColorSliders().at("vLow")->value();
        params.insert(pair<string, int>("vLow", vLow));
        int vHigh = visionTab->getColorSliders().at("vHigh")->value();
        params.insert(pair<string, int>("vHigh", vHigh));

        // get shape params
        int blurSize = visionTab->getShapeSliders().at("blurSize")->value();
        params.insert(pair<string, int>("blurSize", blurSize));
        int edgeThresh = visionTab->getShapeSliders().at("edgeThresh")->value();
        params.insert(pair<string, int>("edgeThresh", edgeThresh));
        int polyError = visionTab->getShapeSliders().at("polyError")->value();
        params.insert(pair<string, int>("polyError", polyError));
        int frontNumVert = visionTab->getShapeSliders().at("frontNumVert")->value();
        params.insert(pair<string, int>("frontNumVert", frontNumVert));
        int frontMinSize = visionTab->getShapeSliders().at("frontMinSize")->value();
        params.insert(pair<string, int>("frontMinSize", frontMinSize));
        int frontMaxSize = visionTab->getShapeSliders().at("frontMaxSize")->value();
        params.insert(pair<string, int>("frontMaxSize", frontMaxSize));
        int backNumVert = visionTab->getShapeSliders().at("backNumVert")->value();
        params.insert(pair<string, int>("backNumVert", backNumVert));
        int backMinSize = visionTab->getShapeSliders().at("backMinSize")->value();
        params.insert(pair<string, int>("backMinSize", backMinSize));
        int backMaxSize = visionTab->getShapeSliders().at("backMaxSize")->value();
        params.insert(pair<string, int>("backMaxSize", backMaxSize));

        Settings settings;

        settings.writeRobotProfile(dialog.textValue(), params);
    }
}
