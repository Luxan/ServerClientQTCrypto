/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../include/server/task_queue.h"
#include "../../include/server/task.h"

void TaskQueue::AddTask(Task *t)
{
    lockJobs.lock();
    jobs.push(t);
    lockJobs.unlock();
}

Task* TaskQueue::GetNextTask()
{
    std::lock_guard<std::mutex> guard(lockJobs);

    if (jobs.empty())
    {
        return nullptr;
    }

    Task * task = jobs.front();
    jobs.pop();

    return task;
}


Task* TaskQueue::GetNextDoneTask()
{
    std::lock_guard<std::mutex> guard(lockDone);

    if (done.empty())
    {
        return nullptr;
    }

    Task * task = done.front();
    done.pop();

    return task;
}

void TaskQueue::AddDoneTask(Task * t)
{
    lockDone.lock();
    done.push(t);
    lockDone.unlock();
}
