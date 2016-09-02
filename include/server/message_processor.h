/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <list>

#include "../server/interface_communication.h"
#include "../server/processor.h"
#include "../shared/message.h"
#include "../server/controller_messageprocessor_messageworker.h"

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

