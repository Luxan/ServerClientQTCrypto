#pragma once

#include <netinet/in.h>
#include <sys/epoll.h>

#include "interface_communication.h"
#include "../../shared/id_session.h"
#include "../../shared/packages/package.h"
#include "../../shared/packages/package_instant_replay.h"
#include "interface_thread.h"
#include "../../shared/user.h"
#include "../../shared/crypto/hasher.h"
#include "../../shared/crypto/cipher.h"
#include "../../shared/crypto/certificate.h"
#include "../../server/clientsocket.h"
#include "../../shared/crypto/key_agreement_agent.h"

/**
\class
\brief
*/
class InterfaceTcpChannel : public InterfaceThread
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
    void prepareToSend(PackageWrapperEncoded *pw);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    InterfaceTcpChannel(ThreadConfiguration conf, int portNumb, int maxEvents);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual ~InterfaceTcpChannel();
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
protected:
    const int portno;
    const int maxEvents;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual void closeSocket(int i) = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual bool initialize() = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual int doListen() = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual bool checkIncomingConnections(int i) = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual void handleNotificationOnListningSocket() = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual bool isSocketValid(int i) = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual bool sendBuffer(uint8_t *buff, size_t size, int i) = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual bool readBuffer(uint8_t *buf, ssize_t aviableToRead, ssize_t &redData, int i) = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void logError(std::string data);
private:
    enum eChannelState
    {
        UnInitialized,
        Listning//,
//        EventsProcessing,
//        NotificationHandling,
//        ProcessingEventData,
//        CloseEpolDescriptor
    };

    std::map<SessionID, std::list<PackageWrapperEncoded*>*> packagesToSend;
    eChannelState state;
    RemoteClient * connectedClients;

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
    void handleSessionDetailRequestPackage(PackageWrapperEncoded *pw, int i);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void processPacketExchange(int i);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void processReceivedBuffers(std::list<PackageBuffer *> &list, int i);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageWrapperEncoded *CreatePackage(PackageBuffer *buf);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool sendPackage(PackageWrapperEncoded * response, int i);
};
