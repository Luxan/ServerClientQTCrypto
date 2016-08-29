/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../include/server/task_queue.h"
#include "../../include/server/task.h"

void TaskQueue::AddTask(Task *t)
{
    lock.lock();
    jobs.push(t);
    lock.unlock();
}

Task* TaskQueue::GetNextTask()
{
    std::lock_guard<std::mutex> guard(lock);

    if (jobs.empty())
    {
        return nullptr;
    }

    Task * task = jobs.front();
    jobs.pop();

    return task;
}
