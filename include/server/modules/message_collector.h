/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <map>
#include <list>

#include "../interfaces/interface_communication.h"
#include "../../shared/packages/package.h"
#include "../../shared/messages/message.h"
#include "../interfaces/interface_thread.h"

/**
\class
\brief
*/
//interface
class MessageCollector : public InterfaceThread
{
private:
    std::map<SessionID, std::list<MessageProcessable *>*> qMessages;

    /**
    \see interface_thread.h
    */
    virtual void dowork();
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    MessageCollector(ThreadConfiguration conf);
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
    void RequestStop();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void storeMessageToSend(Message *m);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void storeKeyAgreementMessageToSend(MessageSessionDetailResponse *m);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void collectPackage(PackageWrapperDecoded *p);

    virtual ~MessageCollector(){}
};
