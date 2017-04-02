#ifndef ROSHANDLER_H
#define ROSHANDLER_H

#include <QThread>
#include <string>
#include "camlistener.h"
#include "shellexecute.h"

class RosHandler : public QThread
{
    Q_OBJECT
public:
    RosHandler(QObject *parent);
    ~RosHandler();
    void setTeamSide(bool inTeamSideHome);
    ShellExecute *getRosNodes();
signals:
    void rosNodesRunning(bool value);
    void rosNodesOutput(std::string text);
public slots:
    void changeRosState(bool isRun);
    void teamSideChanged(bool isHome);
    void shellOutChanged(std::string text);
protected:
    void run();
private:
    ShellExecute *rosNodes;
    bool isRosRunning;
    bool teamSideHome;

    void setUpHandler();
    void shutDownHandler();


};

#endif // ROSHANDLER_H
