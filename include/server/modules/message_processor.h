/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <map>

#include "../interfaces/interface_communication.h"
#include "../../server/interfaces/interface_processor.h"
#include "../../shared/messages/message.h"
#include "../../../include/server/controllers/controller_messageprocessor_threadworker.h"
#include "../../../include/shared/crypto/key_agreement_agent.h"
#include "../../../include/shared/id_session.h"

class Controller_MessageProcessor_ThreadWorker;

/**
\class
\brief
*/
class MessageProcessor : public Processor
{
private:
    std::map<SessionID, KeyAgreamentAgent *> agentBank;
    Certificate * certificate;
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    MessageProcessor(ThreadConfiguration conf, Controller_MessageProcessor_ThreadWorker *controller, int numberOfWorkers, Certificate * certificate);
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
    void AttemptToLogIn(MessagePing * ping);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void AddKeyAgreementMessageToProcess(MessageSessionDetailRequest * m);
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

