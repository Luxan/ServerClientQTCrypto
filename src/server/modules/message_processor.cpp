#include <thread>

/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../include/server/task.h"
#include "../../include/server/message_processor.h"
#include "../../include/server/thread_worker.h"
#include "../../include/server/systemevents.h"
#include "../../include/server/impulse.h"
#include "../../include/shared/message.h"

MessageProcessor::MessageProcessor(ThreadConfiguration conf, int numberOfWorkers):
    interfaceThread(conf)
{
    ThreadConfiguration workersConf;
    workersConf.loopSleepTime = 10;
    workersConf.unsleepReactionTime = 1000;
    workersConf.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    workersConf.responseSleepEvent = eSystemEvent::ResponseSleepMessageWorker;
    workersConf.responseStartEvent = eSystemEvent::ResponseStartMessageWorker;

    CreateWorkers(workersConf, numberOfWorkers);
}

void MessageProcessor::RequestSleep()
{
    if (!isRunning())
    {
        std::string error = "Server is already stopped.";
        AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorMessageProcessor, error));
        return;
    }

    sendEventToWorkers(eSystemEvent::RequestSleepMessageWorker);

    sleepThread();
}

void MessageProcessor::RequestStart()
{
    if (isRunning())
    {
        std::string error = "Server is already running.";
        AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorMessageProcessor, error));
        return;
    }

    sendEventToWorkers(eSystemEvent::RequestStartMessageWorker);

    unSleepThread();
}

void MessageProcessor::dowork()
{
    //TODO implement
}

void MessageProcessor::AddMessageToProcess(MessageProcessable *m)
{
    MessageContext *c = new MessageContext();
    c->m = m;
    MessageTask *task = new MessageTask(c);

    AddTask(task);
}
