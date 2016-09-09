/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <thread>
#include <atomic>
#include "interface_communication.h"

struct ThreadConfiguration
{
    enum SleepLoopMode
    {
        doSleepInsideLoop,
        doNotSleepInsideLoop
    };
    int loopSleepTime;
    int unsleepReactionTime;
    SleepLoopMode sleepLoopMode;
    eSystemEvent responseSleepEvent;
    eSystemEvent responseStartEvent;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    ThreadConfiguration()
    {
        loopSleepTime = 10;
        unsleepReactionTime = 1000;
        sleepLoopMode = doSleepInsideLoop;
        responseSleepEvent = eSystemEvent::Undefined;
        responseStartEvent = eSystemEvent::Undefined;
    }
};

class interfaceThread : public interfaceCommunication
{
private:
    std::atomic_bool isTerminated;
    ThreadConfiguration configuration;
    std::thread th;
    std::atomic_bool doTerminate;
    std::atomic_bool doSleep;
    std::thread::id id;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void loop();
protected:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void sleepThread();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void unSleepThread();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual void dowork() = 0;
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    const std::thread::id getThreadID() const;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    interfaceThread(ThreadConfiguration &conf);

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual ~interfaceThread();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool isRunning() const;
};
