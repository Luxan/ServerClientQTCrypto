/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../include/server/interface_thread.h"

interfaceThread::interfaceThread(ThreadConfiguration conf):
    configuration(conf)
{
    doTerminate = false;
    isTerminated = false;
    doSleep = true;

    th = std::thread(&interfaceThread::loop, this);
}

interfaceThread::~interfaceThread()
{
    doTerminate = true;

    while (!isTerminated)
    {
        std::this_thread::yield();
    }

    th.join();
}

void interfaceThread::loop()
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

bool interfaceThread::isRunning() const
{
    return !doSleep.load() && !isTerminated.load();
}

void interfaceThread::sleepThread()
{
    doSleep = true;
}

void interfaceThread::unSleepThread()
{
    doSleep = false;
}
const std::thread::id interfaceThread::getThreadID() const
{
    return id;
}
