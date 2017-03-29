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
    void writeRobotProfile(QString name, std::map<std::string, int>);
    QDomDocument getProfilesDoc();
    std::map<std::string, int> getRobotProfile(QString name);

private:
    QXmlStreamWriter *openProfileWriter();
    QDomDocument profilesDocument();
    void writeProfileDomDocument(QDomDocument doc);
    void writeDomDocument(QString filename, QDomDocument doc);
    std::map<std::string, int> getMapFromDom(QDomElement profile);
};

#endif // SETTINGS_H
