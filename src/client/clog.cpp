#include "../../include/client/clog.h"

CLog::CLog(const std::string _fileName, const std::string _prefix):
    Log(_fileName, _prefix)
{}

//template < typename T >
Log &CLog::operator <<(const std::string &stream)
{
    std::lock_guard<std::mutex> guard(writerLock);

    writeToFile(getPrefix() + stream);

    return * this;
}

CLog &CLog::logError()
{
    static CLog singleton("error.log", "Error : ");
    return singleton;
}
CLog &CLog::logInfo()
{
    static CLog singleton("info.log", "Info : ");
    return singleton;
}
CLog &CLog::logWarn()
{
    static CLog singleton("warn.log", "Warn : ");
    return singleton;
}
