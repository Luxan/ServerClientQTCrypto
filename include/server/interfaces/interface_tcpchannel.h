#pragma once

#include <netinet/in.h>
#include <sys/epoll.h>

#include "interface_communication.h"
#include "../../shared/id_session.h"
#include "../../shared/packages/package.h"
#include "../../shared/packages/package_instant_replay.h"
#include "interface_thread.h"
#include "../../shared/user.h"
#include "../../server/clientsocket.h"

class InterfaceTcpChannel : public InterfaceThread
{
public:
    void prepareToSend(PackageWrapperEncoded *pw);

    InterfaceTcpChannel(ThreadConfiguration conf, int portNumb, int maxEvents);

    virtual ~InterfaceTcpChannel();

    void RequestStart();

    void RequestStop();
protected:
    const int portno;
    const int maxEvents;
    RemoteClient * connectedClients;

    virtual void closeSocket(int i) = 0;

    virtual bool initialize() = 0;

    virtual int doListen() = 0;

    virtual bool checkIncomingConnections(int i) = 0;

    virtual void handleNotificationOnListningSocket() = 0;

    virtual bool isSocketValid(int i) = 0;

    virtual void processPacketExchange(int i) = 0;

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

    virtual void dowork();

    void handleSessionDetailRequestPackage(PackageWrapperEncoded *pw, int i);


    void processReceivedBuffers(std::list<PackageBuffer *> &list, int i);

    PackageWrapperEncoded *CreatePackage(PackageBuffer *buf);

    bool sendPackage(PackageWrapperEncoded * response, int i);
};
