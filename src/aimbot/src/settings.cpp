#include "settings.h"

#include <iostream>

Settings::Settings()
{

}

// Write a robot profile to the profiles file
void Settings::writeRobotProfile(QString name, std::map<std::string, int> elements)
{
    QDomDocument doc = profilesDocument();
    QDomElement root = doc.documentElement();
    QDomElement robots;
    if(root.firstChildElement(GlobalData::robotGroupName).isNull() == false)
    {
        robots = root.firstChildElement(GlobalData::robotGroupName);
    }
    else
    {
        robots = doc.createElement(GlobalData::robotGroupName);
        root.appendChild(robots);
    }

    if(robots.firstChildElement(name).isNull() == false)
    {
        robots.removeChild(robots.firstChildElement(name));
    }
    QDomElement profile = doc.createElement(name);
    robots.appendChild(profile);

    for (auto const& element : elements)
    {
        QDomElement cur = doc.createElement(QString::fromStdString(element.first));
        QDomText t = doc.createTextNode(QString::number(element.second));
        cur.appendChild(t);
        profile.appendChild(cur);
    }

    writeProfileDomDocument(doc);
}

// Returns an XmlStreamWriter for the profiles file
void Settings::writeProfileDomDocument(QDomDocument doc)
{
    QDir dir(GlobalData::settingsPath);
    if (!dir.exists()) // if the directory doesn't exist
    {
        dir.mkdir(GlobalData::settingsPath);
    }

    QString filename = GlobalData::settingsPath + GlobalData::profilesFile;

    writeDomDocument(filename, doc);
}

// Write a dom document to the filename
void Settings::writeDomDocument(QString filename, QDomDocument doc)
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
QDomDocument Settings::profilesDocument()
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

// Gets the robot profile with the given name
std::map<std::string, int> Settings::getRobotProfile(QString name)
{
    QDomDocument doc = profilesDocument();
    QDomElement root = doc.documentElement();
    QDomElement robots = root.firstChildElement(GlobalData::robotGroupName);
    QDomElement profile = robots.firstChildElement(name);
    return getMapFromDom(profile);
}

// Converts the dom element into a map
std::map<std::string, int> Settings::getMapFromDom(QDomElement profile)
{
    std::map<std::string, int> elements;
    for(int i = 0; i < profile.childNodes().size(); i++)
    {
        QDomNode curNode = profile.childNodes().at(i);
        elements.insert(std::pair<std::string, int>(curNode.nodeName().toStdString(), curNode.firstChild().nodeValue().toInt()));
        ////std::cout << curNode.nodeName().toStdString() + ": " + curNode.firstChild().nodeValue().toStdString() + "\n";
    }
    return elements;
}
