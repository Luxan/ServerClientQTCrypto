/**
\author Sergey Gorokh (ESEGORO)
*/
#include <thread>

#include "../../include/server/thread_worker.h"
#include "../../include/server/task_queue.h"

ThreadWorker::ThreadWorker(ThreadConfiguration conf, TaskQueue *_queue):
    InterfaceThread(conf), queue(_queue)
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
        try
        {
            task->execute();
        }
        catch(std::string error)
        {//TBD create some dump to see what was wrong!
            AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorExecutionTask, error));
            delete task;
            return;
        }
        queue->AddDoneTask(task);
    }
}
