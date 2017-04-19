/**
\author Sergey Gorokh (ESEGORO)
*/
#include <QApplication>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <fstream>

#include "../../include/server/server_initializer.h"
#include "../../include/shared/buffer.h"

void signal_callback_handler(int signum)
{
    modules::deinitializeAll();

    exit(signum);
}

void addUsersTest()
{
    //DataBase::GetDataBase().addUser(std::make_shared<User>(1, "login1", "password"));
}

void setSignalInterception()
{
//    if (signal(SIGHUP, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGHUP";
//    if (signal(SIGINT, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGINT";
//    if (signal(SIGQUIT, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGQUIT";
//    if (signal(SIGILL, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGILL";
//    if (signal(SIGTRAP, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGTRAP";
//    if (signal(SIGABRT, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGABRT";
//    if (signal(SIGIOT, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGIOT";
//    if (signal(SIGBUS, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGBUS";
//    if (signal(SIGFPE, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGFPE";
//    if (signal(SIGKILL, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGKILL";
//    if (signal(SIGSEGV, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGSEGV";
//    if (signal(SIGTERM, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGTERM";
//    if (signal(SIGSTKFLT, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGSTKFLT";
//    if (signal(SIGTSTP, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGTSTP";
//    if (signal(SIGPWR, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGPWR";
//    if (signal(SIGSYS, signal_callback_handler) == SIG_ERR)
//            SLog::logWarn() << " Can't catch SIGSYS";
}

Buffer * loadFile(std::string path)
{
    std::fstream file(path, std::fstream::in | std::ios::binary | std::ios::ate);
    if(file.is_open())
    {
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        Buffer * buf = new Buffer((uint16_t)size);
        file.read((char*)buf->getPointerToBuffer(), size);
        file.close();
        return buf;
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    modules::zero();
    // Register signal and signal handler
    QApplication a(argc, argv);

    //modules
    modules::initializeMainWindow();

    //SLog::logInfo().setGuiLevel(modules::w);

    setSignalInterception();

    Buffer * certificate = loadFile("../../cert/CA/servercert.pem");
    Buffer * privateKey = loadFile("../../cert/CA/private/serverkey.pem");

    modules::initializeEpollTCPChannel(certificate, privateKey);
    controllers::initializeControllers();

    int i = a.exec();
    modules::deinitializeAll();
    delete certificate;
    delete privateKey;
    return i;
}
