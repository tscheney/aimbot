#include "addnewdialog.h"
#include "moc_addnewdialog.cpp"

AddNewDialog::AddNewDialog()
{
    layout = new QGridLayout();

    namesComboBox = new QComboBox();
    profileComboBox = new QComboBox();
    QStringList names = QStringList() << "ally1" << "ally2" << "opp1" << "opp2" << "ball";
    connect(namesComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(namesComboBoxChanged(QString)));
    namesComboBox->addItems(names);

    layout->addWidget(namesComboBox);
    setLayout(layout);
}

// Handle the names combo box changed event
void AddNewDialog::namesComboBoxChanged(QString newName)
{
    //QSettings settings(GlobalData::settingsFolder, GlobalData::profilesFile);
    if(newName == "ball")
    {
        // ball stuff
    }
    else
    {
        //int robotsIndex = settings.childGroups().indexOf(GlobalData::robotGroupName);
        //printf("count %d\n\r", settings.childGroups().);

    }
}
