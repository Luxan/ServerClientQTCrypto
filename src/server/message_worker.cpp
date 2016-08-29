/**
\author Sergey Gorokh (ESEGORO)
*/
#include <thread>

#include "../../include/server/message_worker.h"
#include "../../include/server/task_queue.h"

MessageWorker::MessageWorker(ThreadConfiguration conf, TaskQueue *_queue):
    interfaceThread(conf), queue(_queue)
{}

MessageWorker::~MessageWorker()
{}

void MessageWorker::RequestSleep()
{
    sleepThread();
}

void MessageWorker::RequestStart()
{
    unSleepThread();
}

void MessageWorker::dowork()
{
    Task *task = queue->GetNextTask();

    if (task != nullptr)
    {
        task->execute();
    }
}
