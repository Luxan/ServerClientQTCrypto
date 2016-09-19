/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../include/server/interfaces/interface_thread.h"

InterfaceThread::InterfaceThread(ThreadConfiguration &conf):
    configuration(conf)
{
    doTerminate = false;
    isTerminated = true;
    doSleep = false;
}

InterfaceThread::~InterfaceThread()
{
    terminate();

    if (th.joinable())
        th.join();
}

void InterfaceThread::terminate()
{
    doTerminate = true;


    while (!isTerminated)
    {
        std::this_thread::yield();
    }
}

void InterfaceThread::loop()
{
    id = std::this_thread::get_id();

    while(!doTerminate)
    {
        if(!doSleep)
            AddImpulseToQueue(new ImpulseSignal(configuration.responseStartEvent, std::this_thread::get_id()));

        while(!doSleep && !doTerminate)
        {
            dowork();

            if (configuration.sleepLoopMode == ThreadConfiguration::doSleepInsideLoop)
                std::this_thread::sleep_for(std::chrono::milliseconds(configuration.loopSleepTime));

            CheckAllEvents();

            if(doSleep)
                AddImpulseToQueue(new ImpulseSignal(configuration.responseSleepEvent, std::this_thread::get_id()));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(configuration.unsleepReactionTime));

        CheckAllEvents();
    }
    isTerminated = true;
}

bool InterfaceThread::isRunning() const
{
    return !doSleep.load() && !isTerminated.load();
}

void InterfaceThread::sleepThread()
{
    doSleep = true;
}

void InterfaceThread::startMainLoop()
{
    if(!isTerminated)
    {
        AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorThread, "Thread is not yet stopped to start again!"));
        return;
    }
    doTerminate = false;
    isTerminated = false;
    doSleep = true;

    th = std::thread(&InterfaceThread::loop, this);
}

void InterfaceThread::unSleepThread()
{
    doSleep = false;
}
const std::thread::id InterfaceThread::getThreadID() const
{
    return id;
}
