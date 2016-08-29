#include <thread>

/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../include/server/task.h"
#include "../../include/server/message_processor.h"
#include "../../include/server/message_worker.h"
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

    cmpmw.setModule1Obj(this);
    this->AddEventController(&cmpmw);

    for (int i = 0; i < numberOfWorkers; i++)
    {
        MessageWorker *worker = new MessageWorker(workersConf, &tq);

        cmpmw.setModule2Obj(worker);
        worker->AddEventController(&cmpmw);

        lWorkers.push_back(worker);
    }
}

MessageProcessor::~MessageProcessor()
{
    for (MessageWorker *w : lWorkers)
    {
        delete w;
    }
}

void MessageProcessor::AddTask(Task *task)
{
    tq.AddTask(task);
}

void MessageProcessor::RequestSleep()
{
    if (!isRunning())
    {
        std::string error = "Server is already stopped.";
        AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorMessageProcessor, error));
        return;
    }

    for (MessageWorker *w : lWorkers)
    {
        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestSleepMessageWorker, w->getThreadID()));
    }

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

    for (MessageWorker *w : lWorkers)
    {
        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestStartMessageWorker, w->getThreadID()));
    }

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

    tq.AddTask(task);
}
