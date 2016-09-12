/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <list>

#include "../interfaces/interface_communication.h"
#include "../../server/interfaces/processor.h"
#include "../../shared/messages/message.h"

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
    MessageProcessor(ThreadConfiguration conf, int numberOfWorkers);
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
    void dowork();
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

