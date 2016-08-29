/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include <list>

#include "../server/message_worker.h"
#include "../server/task.h"
#include "../server/task_queue.h"
#include "../server/interface_communication.h"
#include "../server/interface_thread.h"
#include "../shared/message.h"
#include "../server/controller_messageprocessor_messageworker.h"

class MessageWorker;

/**
\class
\brief
*/
class MessageProcessor : public interfaceThread
{
private:
    Controller_MessageProcessor_MessageWorker cmpmw;
    std::list<MessageWorker *> lWorkers;
    TaskQueue tq;
    /**
    \threadsafe using threadsafe TaskQueue tq
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void AddTask(Task * task);

    /**
    \see interface_thread.h
    */
    void dowork();
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    MessageProcessor(ThreadConfiguration conf, int numberOfWorkers);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ~MessageProcessor();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void AddMessageToProcess(MessageProcessable * m);

    /**
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
    void RequestSleep();
};

#endif // MESSAGEPROCESSOR_H

