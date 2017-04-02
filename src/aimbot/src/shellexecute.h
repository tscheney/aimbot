#ifndef SHELLEXECUTE_H
#define SHELLEXECUTE_H

#include <QThread>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include <iostream>


class ShellExecute : public QThread
{
    Q_OBJECT
public:
    ShellExecute(QObject *parent);
    void setCmd(std::string inCmd);
    ~ShellExecute();
signals:
    void shellOut(std::string out);
public slots:
    void runLoop(bool value);
protected:
    void run();
private:
    std::string cmd;

    bool isRunLoop;
    FILE * run;
    int pid;
};

#endif // SHELLEXECUTE_H
