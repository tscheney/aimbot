#include "maintab.h"
#include "moc_maintab.cpp"

MainTab::MainTab(QWidget *parent) : QWidget(parent)
{
    layout = new QGridLayout();
    setUpTeamSide();
    startStopButton = new QPushButton(tr("Start"));
    addNewVisionButton = new QPushButton(tr("Add"));
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(2, 1);
    setUpPreFilter();
    layout->addWidget(startStopButton, 3, 1);
    isStarted = false;
    layout->addWidget(addNewVisionButton,5,1);
    setLayout(layout);

    // Establish connections
    connect(startStopButton, SIGNAL(released()), this, SLOT(startStopButtonPushed()));
    connect(addNewVisionButton, SIGNAL(released()), (MainWindow *) parent, SLOT(addNewClicked()));
    connect(this, SIGNAL(runVision(bool)), (MainWindow *) parent, SLOT(runVision(bool)));
    connect(this, SIGNAL(teamSideChanged(bool)), (MainWindow *) parent, SLOT(teamSideChanged(bool)));
    connect(backSubCheckBox, SIGNAL(toggled(bool)), (MainWindow *) parent, SLOT(useBackSub(bool)));
}

// Add the home/away radial
void MainTab::setUpTeamSide()
{
    QGroupBox *teamSideGroupBox = new QGroupBox(tr("Team Side"), this);
    QHBoxLayout *teamSideLayout = new QHBoxLayout(this);

    homeRadioButton = new QRadioButton("Home", this);
    connect(homeRadioButton, SIGNAL(toggled(bool)), this, SLOT(homeCheckedChanged(bool)));
    homeRadioButton->setChecked(true);
    teamSideLayout->addWidget(homeRadioButton);
    awayRadioButton = new QRadioButton("Away", this);
    teamSideLayout->addWidget(awayRadioButton);

    teamSideGroupBox->setLayout(teamSideLayout);

    layout->addWidget(teamSideGroupBox);
}

void MainTab::setUpPreFilter()
{
    QGroupBox *prefilterGroupBox = new QGroupBox(tr("Prefilter Options"), this);
    QHBoxLayout *prefilterLayout = new QHBoxLayout(this);

    backSubCheckBox = new QCheckBox(tr("Use Background Substraction"), this);
    backSubCheckBox->setChecked(false);

    prefilterLayout->addWidget(backSubCheckBox);
    prefilterGroupBox->setLayout(prefilterLayout);
    layout->addWidget(prefilterGroupBox);
}

MainTab::~MainTab()
{
    delete layout;
    delete addNewVisionButton;
}

// Hanlder home radio button changed event
void MainTab::homeCheckedChanged(bool value)
{
    emit teamSideChanged(value);
}

// Handler for start stop button pushed
void MainTab::startStopButtonPushed()
{
    if(isStarted) // vision nodes already running
    {
        isStarted = false;
        startStopButton->setText(tr("Start"));
        emit runVision(false);
        homeRadioButton->setEnabled(true);
        awayRadioButton->setEnabled(true);
    }
    else
    {
        isStarted = true;
        startStopButton->setText(tr("Stop"));
        emit runVision(true);
        homeRadioButton->setEnabled(false);
        awayRadioButton->setEnabled(false);
    }
}
