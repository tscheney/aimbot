#include "roshandler.h"
#include "moc_roshandler.cpp"

RosHandler::RosHandler(QObject *parent) : QThread(parent)
{
    isRosRunning = false;
    rosNodes = new ShellExecute(this);
    connect(rosNodes, SIGNAL(shellOut(std::string)), this, SLOT(shellOutChanged(std::string)));
    teamSideHome = true;
}

RosHandler::~RosHandler()
{
    delete rosNodes;
}

void RosHandler::run()
{

}

// Set up the node handler
void RosHandler::setUpHandler()
{
    std::string cmd = "roslaunch aimbot centralfromvision.launch";

    if(!teamSideHome)
    {
        cmd += " team_side:=away";
    }
    rosNodes->setCmd(cmd);
    rosNodes->start();

    isRosRunning = true;
    emit rosNodesRunning(isRosRunning);
}

// Shut down the node handler
void RosHandler::shutDownHandler()
{
    rosNodes->quit();
    rosNodes->wait();
    rosNodes->terminate();
    isRosRunning = false;
    emit rosNodesRunning(isRosRunning);
}

// Sets whether team side is home or not
void RosHandler::teamSideChanged(bool isHome)
{
    teamSideHome = isHome;
}

// Returns a pointer to the ros nodes
ShellExecute * RosHandler::getRosNodes()
{
    return rosNodes;
}

// Change ros state
void RosHandler::changeRosState(bool isRun)
{
    if(isRosRunning)
    {
        std::cout << "attempting to delete\n";
        shutDownHandler();
    }

    if(isRun)
    {
        setUpHandler();
    }
}

// Get new text from shell
void RosHandler::shellOutChanged(std::string text)
{
    emit rosNodesOutput(text);
}
