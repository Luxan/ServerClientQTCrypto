/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef MESSAGEWORKER_H
#define MESSAGEWORKER_H

#include <thread>
#include <mutex>
#include <atomic>

#include "../server/task_queue.h"
#include "../server/interface_thread.h"
#include "../server/interface_communication.h"
#include "../server/message_processor.h"

class MessageProcessor;

/**
\class
\brief
*/
class MessageWorker : public interfaceThread
{
private:
    TaskQueue * queue;
    /**
    \see interface_thread.h
    */
    void dowork();
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
    MessageWorker(ThreadConfiguration conf, TaskQueue * _queue);
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~MessageWorker();
};

#endif // MESSAGEWORKER_H
