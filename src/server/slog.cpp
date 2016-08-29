/**
\author Sergey Gorokh (ESEGORO)
*/
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <atomic>

#include "../../include/server/mainwindow.h"
#include "../../include/server/slog.h"

MainWindow * SLog::mw = nullptr;

SLog::SLog(const std::string _fileName, const std::string _prefix):
    Log(_fileName, _prefix)
{}

void SLog::setGuiLevel(MainWindow * _mw)
{
    mw = _mw;
}

//template < typename T >
Log &SLog::operator <<(const std::string &stream)
{
    std::lock_guard<std::mutex> guard(writerLock);

    writeToFile(getPrefix() + stream);

    if (SLog::mw != nullptr)
        MainWindow::putMSG(SLog::mw, getPrefix() + stream);

    return * this;
}

SLog &SLog::logError()
{
    static SLog singleton("error.log", "Error : ");
    return singleton;
}
SLog &SLog::logInfo()
{
    static SLog singleton("info.log", "Info : ");
    return singleton;
}
SLog &SLog::logWarn()
{
    static SLog singleton("warn.log", "Warn : ");
    return singleton;
}
