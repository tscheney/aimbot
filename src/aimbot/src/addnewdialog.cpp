#include "addnewdialog.h"
#include "moc_addnewdialog.cpp"

AddNewDialog::AddNewDialog()
{
    setWindowTitle("Add New Vision");
    layout = new QGridLayout();

    // Add combo boxes
    namesComboBox = new QComboBox(this);
    QStringList names = QStringList() << "ally1" << "ally2" << "opp1" << "opp2" << "ball";
    namesComboBox->addItems(names);
    connect(namesComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(namesComboBoxChanged(QString)));
    profileComboBox = new QComboBox(this);

    // Add button box
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    // Add widgets to layout
    layout->addWidget(new QLabel(tr("Name:"), this, 0),0,0);
    layout->addWidget(new QLabel(tr("Profile:"), this, 0),0,1);
    layout->addWidget(namesComboBox,1,0);
    layout->addWidget(profileComboBox,1,1);
    layout->addWidget(buttonBox,2,0,1,2);
    setLayout(layout);
    namesComboBoxChanged(names.at(0));
}

// Handle the names combo box changed event
void AddNewDialog::namesComboBoxChanged(QString newName)
{
    profileComboBox->clear();
    profileComboBox->addItem(GlobalData::newProfileName);
    Settings settings;
    QDomDocument profilesDoc = settings.getProfilesDoc();
    QDomElement root = profilesDoc.documentElement();
    QDomElement element = root.firstChildElement(GlobalData::robotGroupName);
    if(newName == "ball")
    {
         element = root.firstChildElement(GlobalData::ballGroupName);
    }
    else
    {

         element = root.firstChildElement(GlobalData::robotGroupName);
    }

    // Add to combobox
    if(element.isNull() == false)
    {
        for(int i = 0; i < element.childNodes().size(); i++)
        {
            profileComboBox->addItem(element.childNodes().at(i).nodeName());
        }
    }
}

// Get the current name from the names combobox
QString AddNewDialog::getName()
{
    return namesComboBox->currentText();
}

// Get the current profile form the profile combobox
QString AddNewDialog::getProfile()
{
    return profileComboBox->currentText();
}



