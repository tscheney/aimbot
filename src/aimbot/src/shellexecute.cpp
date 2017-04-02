#include "shellexecute.h"
#include "moc_shellexecute.cpp"

ShellExecute::ShellExecute(QObject *parent) : QThread(parent)
{
    cmd = "";
    isRunLoop = true;
}

void ShellExecute::setCmd(std::string inCmd)
{
    cmd = inCmd;
}

ShellExecute::~ShellExecute()
{
    quit();
    wait();
}

// Threads main function to run a process
void ShellExecute::run()
{
    //std::cout << "starting shell\n\r";
    const int arraySize = 128;
    //std::array<char, arraySize> buffer;
    run = popen2(command, "r", pid);
    //std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    //if (!pipe) throw std::runtime_error("popen() failed!");
    //std::cout << "starting loop\n\r";
    //while (!feof(pipe.get()) && isRunLoop)
    //{
        //std::cout << "shell loop\n\r";
        //if (fgets(buffer.data(), arraySize, pipe.get()) != NULL)
        //{
        //    emit(std::string(buffer.data(), arraySize));
        //    std::cout << std::string(buffer.data(), arraySize) + "\n\r";
        //}
    //}

    // run a process and create a streambuf that reads its stdout and stderr

}

// Sets whether to run the loop
void ShellExecute::runLoop(bool value)
{
    isRunLoop = value;
}


FILE * ShellExecute::popen2(string command, string type, int & pid)
{
    const int READ = 0;
    const int WRITE = 1;
    pid_t child_pid;
    int fd[2];
    pipe(fd);

    if((child_pid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }

    /* child process */
    if (child_pid == 0)
    {
        if (type == "r")
        {
            close(fd[READ]);    //Close the READ end of the pipe since the child's fd is write-only
            dup2(fd[WRITE], 1); //Redirect stdout to pipe
        }
        else
        {
            close(fd[WRITE]);    //Close the WRITE end of the pipe since the child's fd is read-only
            dup2(fd[READ], 0);   //Redirect stdin to pipe
        }

        setpgid(child_pid, child_pid); //Needed so negative PIDs can kill children of /bin/sh
        execl("/bin/sh", "/bin/sh", "-c", command.c_str(), NULL);
        exit(0);
    }
    else
    {
        if (type == "r")
        {
            close(fd[WRITE]); //Close the WRITE end of the pipe since parent's fd is read-only
        }
        else
        {
            close(fd[READ]); //Close the READ end of the pipe since parent's fd is write-only
        }
    }

    pid = child_pid;

    if (type == "r")
    {
        return fdopen(fd[READ], "r");
    }

    return fdopen(fd[WRITE], "w");
}

int ShellExecute::pclose2(FILE * fp, pid_t pid)
{
    int stat;

    fclose(fp);
    while (waitpid(pid, &stat, 0) == -1)
    {
        if (errno != EINTR)
        {
            stat = -1;
            break;
        }
    }

    return stat;
}


int pclose2(FILE * fp, pid_t pid)
{
    int stat;

    fclose(fp);
    while (waitpid(pid, &stat, 0) == -1)
    {
        if (errno != EINTR)
        {
            stat = -1;
            break;
        }
    }

    return stat;
}
