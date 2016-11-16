#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <ostream>
#include <sstream>

#include <QTextEdit>

#define DETAILLOG(Logger_, Message_)            \
  Logger_(                                      \
    static_cast<std::ostringstream&>(           \
      std::ostringstream().flush() << Message_  \
    ).str(),                                    \
    __FUNCTION__,                               \
    __FILE__,                                   \
    __LINE__                                    \
  );

#define LOG(Logger_, Message_)                  \
  Logger_(                                      \
    static_cast<std::ostringstream&>(           \
      std::ostringstream().flush() << Message_  \
    ).str()                                     \
  );

class LogSink
{
public:
    virtual void flushData(std::string const& message) = 0;
};

class Console : public LogSink
{
public:
    virtual void flushData(std::string const& message)
    {
        std::cout << message;
    }
};

class File : public LogSink
{
   std::fstream m_file;
public:
    File(std::string filename)
    {
        m_file.open(filename, std::ios::out);
    }

    virtual void flushData(std::string const& message)
    {
        m_file << message;
    }

    ~File()
    {
        m_file.close();
    }
};

class ApplcationInfo: LogSink
{
    QTextEdit * te;
public:
    ApplcationInfo(QTextEdit * te):
        te(te)
    {}

    virtual void flushData(std::string const& message)
    {
        te->append(QString(message.c_str()));
    }
};

class ApplcationPopUp: LogSink
{
public:
    virtual void flushData(std::string const& message)
    {
        std::cout << message;
    }
};

class Logger
{
private:
    LogSink &ls;
public:
    Logger(LogSink& ls):
        ls(ls)
    {}

    void operator()(std::string const& message,
                    char const* function,
                    char const* file,
                    int line)
    {
        ls.flushData(message + " functon[" + function + "] file[" + file + "] line[" + std::to_string(line) + "]");
    }

    void operator()(std::string const& message)
    {
        ls.flushData(message);
    }

    static Logger& console() {
        static Console console;
        static Logger logger(console);
        return logger;
    }

    static Logger& fileServer() {
        static File file("server.log");
        static Logger logger(file);
        return logger;
    }

    static Logger& fileClient() {
        static File file("client.log");
        static Logger logger(file);
        return logger;
    }
};

