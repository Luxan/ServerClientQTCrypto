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
    std::mutex lockJobs;
    std::mutex lockDone;
    std::queue<Task*> jobs;
    std::queue<Task*> done;
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

    Task * GetNextDoneTask();

    /**
    \threadsafe using std::mutex lock to manage access to std::queue<Task*> done
    \param t - task that has been done.
    \throw
    \brief
    \pre
    \post
    */
    void AddDoneTask(Task * t);
};

#endif // TASKQUEUE_H
