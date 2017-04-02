#include "maintab.h"
#include "moc_maintab.cpp"

MainTab::MainTab(QWidget *parent) : QWidget(parent)
{
    isRosRunning = false;
    layout = new QGridLayout();
    addNewVisionButton = new QPushButton(tr("Add"));
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(2, 1);
    layout->addWidget(addNewVisionButton,1,1);
    setUpRosNode();
    setLayout(layout);

    // Establish connections
    connect(addNewVisionButton, SIGNAL(released()), (MainWindow *) parent, SLOT(addNewClicked()));
}

MainTab::~MainTab()
{
    delete layout;
    delete addNewVisionButton;
}

// Set up gui options for ros nodes
void MainTab::setUpRosNode()
{
    startRosNodesButton = new QPushButton(tr("Start Nodes"), this);
    connect(startRosNodesButton, SIGNAL(released()), this, SLOT(startRosNodesClicked()));
    QGroupBox *rosNodeGroupBox = new QGroupBox(tr("Ros Nodes"));
    QGridLayout *rosNodeLayout = new QGridLayout();
    QGroupBox *teamSideGroupBox = new QGroupBox(tr("Team Side"), this);
    QHBoxLayout *teamSideLayout = new QHBoxLayout(this);

    //QLabel *homeLabel = new QLabel(tr("Home"));
    homeRadioButton = new QRadioButton("Home", this);
    connect(homeRadioButton, SIGNAL(toggled(bool)), this, SLOT(homeCheckedChanged(bool)));
    homeRadioButton->setChecked(true);
    teamSideLayout->addWidget(homeRadioButton);
    awayRadioButton = new QRadioButton("Away", this);
    teamSideLayout->addWidget(awayRadioButton);

    teamSideGroupBox->setLayout(teamSideLayout);
    rosNodeLayout->addWidget(teamSideGroupBox,0,0,2,2);
    rosNodeLayout->addWidget(startRosNodesButton,1,3);


    outputLabel = new QLabel();
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(outputLabel);
    rosNodeLayout->addWidget(scrollArea, 2, 0, 5, 5);

    rosNodeGroupBox->setLayout(rosNodeLayout);
    layout->addWidget(rosNodeGroupBox,3,0,5,5);
}

// Handle ros nodes clicked event
void MainTab::startRosNodesClicked()
{
    if(isRosRunning) // ros node running, stop them
    {
        homeRadioButton->setEnabled(true);
        awayRadioButton->setEnabled(true);
        startRosNodesButton->setText("Start Nodes");
        emit changeRosState(false);


    }
    else // ros nodes not running, start them
    {
        homeRadioButton->setEnabled(false);
        awayRadioButton->setEnabled(false);
        startRosNodesButton->setText("Stop Nodes");
        emit changeRosState(true);
    }
}

// Receive ros node running signal from ros handler
void MainTab::rosNodesRunning(bool value)
{
    isRosRunning = value;
}

// Hanlder home radio button changed event
void MainTab::homeCheckedChanged(bool value)
{
    emit teamSideChanged(value);
}

void MainTab::rosNodesOutput(std::string text)
{
    outputLabel->setText(outputLabel->text() + QString::fromStdString(text));
}
