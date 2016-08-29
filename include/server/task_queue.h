/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <queue>
#include <mutex>

#include "../server/task.h"

/**
\class
\brief
\threadsafe all functions are threadsafe!
*/
class TaskQueue
{
private:
    std::mutex lock;
    std::queue<Task*> jobs;
public:
    /**
    \threadsafe using std::mutex lock to manage access to std::queue<Task*> jobs
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void AddTask(Task * t);
    /**
    \threadsafe using std::mutex lock to manage access to std::queue<Task*> jobs
    \param
    \return task if queue is not empty. othervise returns nullptr
    \throw
    \brief
    \pre
    \post
    */
    Task * GetNextTask();
};

#endif // TASKQUEUE_H
