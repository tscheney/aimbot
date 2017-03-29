#include "maintab.h"
#include "moc_maintab.cpp"

MainTab::MainTab(QWidget *parent) : QWidget(parent)
{
    layout = new QGridLayout();
    addNewVisionButton = new QPushButton(tr("Add"));
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(2, 1);
    layout->addWidget(addNewVisionButton,1,1);
    setLayout(layout);

    // Establish connections
    connect(addNewVisionButton, SIGNAL(released()), (MainWindow *) parent, SLOT(addNewClicked()));
}

MainTab::~MainTab()
{
    delete layout;
    delete addNewVisionButton;
}
