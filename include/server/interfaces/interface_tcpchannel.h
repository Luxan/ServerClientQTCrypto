#pragma once

#include <netinet/in.h>
#include <sys/epoll.h>

#include "interface_communication.h"
#include "../../shared/packages/package.h"
#include "interface_thread.h"
#include "../../shared/user.h"
#include "../../shared/crypto/hasher.h"
#include "../../shared/crypto/cipher.h"
#include "../../shared/crypto/certificate.h"
#include "../../server/clientsocket.h"

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
    void prepareToSend(PackageWrapper * pw);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void setCrypto(Hasher *hasher, Cipher * cipher);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    InterfaceTcpChannel(ThreadConfiguration conf, int portNumb, int maxEvents, Certificate * certificate);
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

    Hasher * hasher;
    Cipher * cipher;
    RemoteClient *connectedClients;
    eChannelState state;
    Certificate * certificate;



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
    bool sendPackageMultyMessage(PackageWrapper::ePackageType type, PackageMultiPackage *mp, int i);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool sendStrictSizePackage(PackageStrictSize * sp, uint8_t strictSize, PackageWrapper::ePackageType type, int i);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void handleLoginPackage(Package * p, int i);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void handleSessionDetailRequestPackage(int i);
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
    PackageWrapper *CreatePackage(PackageBuffer *buf);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool sendPackage(PackageWrapper * response, int i);
};
