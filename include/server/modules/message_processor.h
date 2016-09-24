/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <list>

#include "../interfaces/interface_communication.h"
#include "../../server/interfaces/interface_processor.h"
#include "../../shared/messages/message.h"
#include "../../../include/server/controllers/controller_messageprocessor_threadworker.h"

class Controller_MessageProcessor_ThreadWorker;

/**
\class
\brief
*/
class MessageProcessor : public Processor
{
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    MessageProcessor(ThreadConfiguration conf, Controller_MessageProcessor_ThreadWorker *controller, int numberOfWorkers);
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
    void missingMessage(Message * m);
    /**
    \see interface_thread.h
    */
    virtual void dowork();
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

    virtual ~MessageProcessor(){}
};

