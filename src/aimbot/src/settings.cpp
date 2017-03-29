#include "settings.h"

#include <iostream>

Settings::Settings()
{

}

// Write a profile to the profiles file
void Settings::writeProfile(QString name, QString groupName, std::map<std::string, int> elements)
{
    QDomDocument doc = createProfilesDoc();
    QDomElement root = doc.documentElement();
    QDomElement group;
    if(root.firstChildElement(groupName).isNull() == false)
    {
        group = root.firstChildElement(groupName);
    }
    else
    {
        group = doc.createElement(groupName);
        root.appendChild(group);
    }

    if(group.firstChildElement(name).isNull() == false)
    {
        group.removeChild(group.firstChildElement(name));
    }
    QDomElement profile = doc.createElement(name);
    group.appendChild(profile);

    for (auto const& element : elements)
    {
        QDomElement cur = doc.createElement(QString::fromStdString(element.first));
        QDomText t = doc.createTextNode(QString::number(element.second));
        cur.appendChild(t);
        profile.appendChild(cur);
    }

    writeProfileDomDoc(doc);
}

// Write a robot profile to the profiles file
void Settings::writeRobotProfile(QString name, std::map<std::string, int> elements)
{
    writeProfile(name, GlobalData::robotGroupName, elements);
}

// Write a ball profile to the profiles file
void Settings::writeBallProfile(QString name, std::map<std::string, int> elements)
{
    writeProfile(name, GlobalData::ballGroupName, elements);
}

// Returns an XmlStreamWriter for the profiles file
void Settings::writeProfileDomDoc(QDomDocument doc)
{
    QDir dir(GlobalData::settingsPath);
    if (!dir.exists()) // if the directory doesn't exist
    {
        dir.mkdir(GlobalData::settingsPath);
    }

    QString filename = GlobalData::settingsPath + GlobalData::profilesFile;

    writeDomDoc(filename, doc);
}

// Write a dom document to the filename
void Settings::writeDomDoc(QString filename, QDomDocument doc)
{
    QFile file(filename);
    QByteArray xml = doc.toByteArray();
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(xml);
        file.close();
    }
}

// Create a new profiles document
QDomDocument Settings::createProfilesDoc()
{
    QDomDocument doc("profiles");
    QString filename = GlobalData::settingsPath + GlobalData::profilesFile;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        doc.setContent(&file);
        file.close();
    }
    else
    {
        QDomElement root = doc.createElement("profiles");
        doc.appendChild(root);
    }

    return doc;
}

// Get the current profiles document
QDomDocument Settings::getProfilesDoc()
{
    QDomDocument doc("profiles");
    QString filename = GlobalData::settingsPath + GlobalData::profilesFile;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        doc.setContent(&file);
        file.close();
    }
    return doc;
}

// Gets the profile with the given name from the given group
std::map<std::string, int> Settings::getProfile(QString name, QString groupName)
{
    QDomDocument doc = createProfilesDoc();
    QDomElement root = doc.documentElement();
    QDomElement group = root.firstChildElement(groupName);
    QDomElement profile = group.firstChildElement(name);
    return getMapFromDom(profile);
}

// Gets the robot profile with the given name
std::map<std::string, int> Settings::getRobotProfile(QString name)
{
    return getProfile(name, GlobalData::robotGroupName);
}

// Gets the ball profile with the given name
std::map<std::string, int> Settings::getBallProfile(QString name)
{
    return getProfile(name, GlobalData::ballGroupName);
}

// Converts the dom element into a map
std::map<std::string, int> Settings::getMapFromDom(QDomElement profile)
{
    std::map<std::string, int> elements;
    for(int i = 0; i < profile.childNodes().size(); i++)
    {
        QDomNode curNode = profile.childNodes().at(i);
        elements.insert(std::pair<std::string, int>(curNode.nodeName().toStdString(), curNode.firstChild().nodeValue().toInt()));
    }
    return elements;
}
