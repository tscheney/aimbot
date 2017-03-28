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
    MainTab *mainTab = new MainTab();
    mainTab->setObjectName("Main");
    tabs->addTab(mainTab, tr("Main"));
    setCentralWidget(tabs);
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    setUpMenuBar();

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

// Set up the menu bar for the main window
void MainWindow::setUpMenuBar()
{
    fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *addVision = new QAction(tr("&Add Vision..."), this);
    addVision->setObjectName("addVision");
    fileMenu->addAction(addVision);
    MainTab *mainTab = tabs->findChild<MainTab *>("Main");
    connect(addVision, SIGNAL(triggered()), mainTab, SLOT(addNewClicked()));

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

// Inserts a new tab into the tabs container
void MainWindow::insertNewTab(QString name)
{
    VisionTab *visionTab = new VisionTab(this, name);
    visionTab->setObjectName(name);
    connect(camListener, SIGNAL(rawImage(cv::Mat)), visionTab->getVision(), SLOT(process(cv::Mat)));
    tabs->addTab(visionTab, name);
    tabs->setCurrentIndex(tabs->count() - 1);
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

// Save settings
void MainWindow::saveClicked()
{
    std::cout << "save clicked\n";
    QInputDialog dialog(this, Qt::Dialog);
    dialog.setWindowTitle("Save Settings");
    dialog.setLabelText("Enter a name:");
    if(dialog.exec() == QDialog::Accepted)
    {


        //printf("ok");
        VisionTab *visionTab = (VisionTab *) tabs->currentWidget();
        //string prefix = "robot/" + dialog.textValue().toStdString() + "/";
        std::cout << "begin vision tab get\n";
        // get color params
        int hLow = visionTab->getColorSliders().at("hLow")->value();
        int hHigh = visionTab->getColorSliders().at("hHigh")->value();
        int sLow = visionTab->getColorSliders().at("sLow")->value();
        int sHigh = visionTab->getColorSliders().at("sHigh")->value();
        int vLow = visionTab->getColorSliders().at("vLow")->value();
        int vHigh = visionTab->getColorSliders().at("vHigh")->value();
        std::cout << "one down\n";

        // get shape params
        int blurSize = visionTab->getShapeSliders().at("blurSize")->value();
        int edgeThresh = visionTab->getShapeSliders().at("edgeThresh")->value();
        int polyError = visionTab->getShapeSliders().at("polyError")->value();
        int frontNumVert = visionTab->getShapeSliders().at("frontNumVert")->value();
        int frontMinSize = visionTab->getShapeSliders().at("frontMinSize")->value();
        int frontMaxSize = visionTab->getShapeSliders().at("frontMaxSize")->value();
        int backNumVert = visionTab->getShapeSliders().at("backNumVert")->value();
        int backMinSize = visionTab->getShapeSliders().at("backMinSize")->value();
        int backMaxSize = visionTab->getShapeSliders().at("backMaxSize")->value();

        std::cout << "begin xml\n";
        QString filename = GlobalData::settingsPath + GlobalData::profilesFile;
        std::cout << filename.toStdString();

        QFile file(filename);
        file.open(QIODevice::WriteOnly);

        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement(GlobalData::robotGroupName);
        xmlWriter.writeStartElement(dialog.textValue());

        std::cout << "being writing elements\n";
        // write color params to settings
        xmlWriter.writeTextElement("hLow", QString(hLow));
        xmlWriter.writeTextElement("hHigh", QString(hHigh));
        xmlWriter.writeTextElement("sLow", QString(sLow));
        xmlWriter.writeTextElement("sHigh", QString(sHigh));
        xmlWriter.writeTextElement("vLow", QString(vLow));
        xmlWriter.writeTextElement("vHigh", QString(vHigh));

        // write shape params to settings
        xmlWriter.writeTextElement("blurSize", QString(blurSize));
        xmlWriter.writeTextElement("edgeThresh", QString(edgeThresh));
        xmlWriter.writeTextElement("polyError", QString(polyError));
        xmlWriter.writeTextElement("frontNumVert", QString(frontNumVert));
        xmlWriter.writeTextElement("frontMinSize", QString(frontMinSize));
        xmlWriter.writeTextElement("frontMaxSize", QString(frontMaxSize));
        xmlWriter.writeTextElement("backNumVert", QString(backNumVert));
        xmlWriter.writeTextElement("backMinSize", QString(backMinSize));
        xmlWriter.writeTextElement("backMaxSize", QString(backMaxSize));

        xmlWriter.writeEndElement();
        xmlWriter.writeEndElement();

        file.close();
    }
}

//// Save settings
//void MainWindow::saveClicked()
//{

//    QSettings settings(GlobalData::settingsFolder, GlobalData::profilesFile);
//    QInputDialog dialog(this, Qt::Dialog);
//    dialog.setWindowTitle("Save Settings");
//    dialog.setLabelText("Enter a name:");
//    if(dialog.exec() == QDialog::Accepted)
//    {
//        //printf("ok");
//        VisionTab *visionTab = (VisionTab *) tabs->currentWidget();
//        //string prefix = "robot/" + dialog.textValue().toStdString() + "/";

//        // get color params
//        int hLow = visionTab->getColorSliders().at("hLow")->value();
//        int hHigh = visionTab->getColorSliders().at("hHigh")->value();
//        int sLow = visionTab->getColorSliders().at("sLow")->value();
//        int sHigh = visionTab->getColorSliders().at("sHigh")->value();
//        int vLow = visionTab->getColorSliders().at("vLow")->value();
//        int vHigh = visionTab->getColorSliders().at("vHigh")->value();

//        // get shape params
//        int blurSize = visionTab->getShapeSliders().at("blurSize")->value();
//        int edgeThresh = visionTab->getShapeSliders().at("edgeThresh")->value();
//        double polyError = visionTab->getShapeSliders().at("polyError")->value();
//        int frontNumVert = visionTab->getShapeSliders().at("frontNumVert")->value();
//        int frontMinSize = visionTab->getShapeSliders().at("frontMinSize")->value();
//        int frontMaxSize = visionTab->getShapeSliders().at("frontMaxSize")->value();
//        int backNumVert = visionTab->getShapeSliders().at("backNumVert")->value();
//        int backMinSize = visionTab->getShapeSliders().at("backMinSize")->value();
//        int backMaxSize = visionTab->getShapeSliders().at("backMaxSize")->value();

//        settings.beginGroup(GlobalData::robotGroupName);

//        settings.beginGroup(dialog.textValue());
//        // write color params to settings
//        settings.setValue("hLow", hLow);
//        settings.setValue("hHigh", hHigh);
//        settings.setValue("sLow", hLow);
//        settings.setValue("sHigh", hHigh);
//        settings.setValue("vLow", hLow);
//        settings.setValue("vHigh", hHigh);

//        // write shape params to settings
//        settings.setValue("blurSize", blurSize);
//        settings.setValue("edgeThresh", edgeThresh);
//        settings.setValue("polyError", polyError);
//        settings.setValue("frontNumVert", frontNumVert);
//        settings.setValue("frontMinSize", frontMinSize);
//        settings.setValue("frontMaxSize", frontMaxSize);
//        settings.setValue("backNumVert", backNumVert);
//        settings.setValue("backMinSize", backMinSize);
//        settings.setValue("backMaxSize", backMaxSize);
//        settings.endGroup();

//        settings.endGroup();
//    }
//}
