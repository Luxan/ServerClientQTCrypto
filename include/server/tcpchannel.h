/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef SERVERTCP_H
#define SERVERTCP_H

#include <netinet/in.h>
#include <sys/epoll.h>

#include "../server/interface_communication.h"
#include "../shared/package.h"
#include "../server/interface_thread.h"
#include "../shared/user.h"
#include "../shared/crypto.h"
#include "../server/clientsocket.h"

class interfaceCommunication;

/**
\class
\brief
*/
class TcpChannel : public interfaceThread
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
    void setCipher(Hasher *hasher, Cipher * cipher);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    TcpChannel(ThreadConfiguration conf,int portNumb, int MaxEvents);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual ~TcpChannel();
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
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool initialize(epoll_event &event);
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
    int create_and_bind(int port);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    int make_socket_non_blocking(int sfd);

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
    void handleNotificationOnListningSocket();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool readBuffer(int i, uint8_t *buf, ssize_t &count);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool sendPackage(PackageWrapper * response, int i);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool sendBuffer(uint8_t *buff, size_t size, int i);
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
    void handleKeyRequestPackage(int i);
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
    void LogError(std::string data);

    eChannelState state;

    struct epoll_event event;
    const int maxEvents;
    const int portno;
    int sfd;
    int efd;
    struct epoll_event *events;
    ClientSocket *connectedClients;
    Hasher * hasher;
    Cipher * cipher;
};

#endif // SERVERTCP_H
