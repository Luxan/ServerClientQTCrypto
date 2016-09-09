/**
\author Sergey Gorokh (ESEGORO)
*/
#include <thread>

#include "../../include/server/message_worker.h"
#include "../../include/server/task_queue.h"

ThreadWorker::ThreadWorker(ThreadConfiguration conf, TaskQueue *_queue):
    interfaceThread(conf), queue(_queue)
{}

ThreadWorker::~ThreadWorker()
{}

void ThreadWorker::RequestSleep()
{
    sleepThread();
}

void ThreadWorker::RequestStart()
{
    unSleepThread();
}

void ThreadWorker::dowork()
{
    Task *task = queue->GetNextTask();

    if (task != nullptr)
    {
        task->execute();
    }
}
