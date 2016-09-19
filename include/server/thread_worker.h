/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef MESSAGEWORKER_H
#define MESSAGEWORKER_H

#include <thread>
#include <mutex>
#include <atomic>

#include "task_queue.h"
#include "interfaces/interface_thread.h"
#include "interfaces/interface_communication.h"

/**
\class
\brief
*/
class ThreadWorker : public InterfaceThread
{
private:
    TaskQueue * queue;
    /**
    \see interface_thread.h
    */
    virtual void dowork();
public:
    /**
    \threadsafe using interfaceThread and std::atomic_bool stopExecuted
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void RequestSleep();
    /**
    \threadsafe using interfaceThread and std::atomic_bool stopExecuted
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void RequestStart();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ThreadWorker(ThreadConfiguration conf, TaskQueue * _queue);
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~ThreadWorker();
};

#endif // MESSAGEWORKER_H
