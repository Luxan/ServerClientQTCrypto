/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef MESSAGECOLLECTOR_H
#define MESSAGECOLLECTOR_H

#include "../server/interface_communication.h"
#include "../shared/package.h"
#include "../shared/message.h"
#include "../server/interface_thread.h"

/**
\class
\brief
*/
//interface
class MessageCollector : public interfaceThread
{
private:
    std::queue<MessageProcessable *> qMessages;

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
    void CollectPackage(PackageWrapper *p);
};

#endif // MESSAGECOLLECTOR_H
