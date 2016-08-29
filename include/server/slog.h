/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef CLOG_H
#define CLOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

#include "../server/mainwindow.h"
#include "../shared/log.h"

class LoginWindow;

/**
\class
\brief
*/
class SLog : Log
{
private:
    static MainWindow * mw;

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    SLog(std::string _fileName, std::string _prefix);

public:

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void setGuiLevel(MainWindow * _mw);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    //template < typename T >
    virtual Log &operator <<(const std::string &stream) override;

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    static SLog &logError();

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    static SLog &logInfo();

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    static SLog &logWarn();
};

#endif // CLOG_H
