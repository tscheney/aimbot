#include "maintab.h"
#include "moc_maintab.cpp"

MainTab::MainTab(QWidget *parent) : QWidget(parent)
{
    layout = new QGridLayout();
    addNewVision = new QPushButton(tr("Add"));
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(2, 1);
    layout->addWidget(addNewVision,1,1);
    setLayout(layout);

    // Establish connections
    connect(addNewVision, SIGNAL(released()), this, SLOT(addNewClicked()));
}

MainTab::~MainTab()
{
    delete layout;
    delete addNewVision;
}

void MainTab::addNewClicked()
{
    QInputDialog dialog(this, Qt::Dialog);
    if(dialog.exec() == QDialog::Accepted)
    {
       emit addNewTab(dialog.textValue());
    }

    //AddNewDialog dialog;
    //dialog.exec();



//    if(dialog.exec() == QDialog::Accepted)
//    {
//        //emit addNewTab(dialog.textValue());
//    }
}
