#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QtXml>
#include <string>
#include <map>
#include "global.h"

class Settings
{
public:
    Settings();
    void writeRobotProfile(QString name, std::map<std::string, int> elements);
    void writeBallProfile(QString name, std::map<std::string, int> elements);
    QDomDocument getProfilesDoc();
    std::map<std::string, int> getRobotProfile(QString name);
    std::map<std::string, int> getBallProfile(QString name);

private:
    QDomDocument createProfilesDoc();
    void writeProfileDomDoc(QDomDocument doc);
    void writeDomDoc(QString filename, QDomDocument doc);
    void writeProfile(QString name, QString groupName, std::map<std::string, int> elements);
    std::map<std::string, int> getMapFromDom(QDomElement profile);
    std::map<std::string, int> getProfile(QString name, QString groupName);
};

#endif // SETTINGS_H
