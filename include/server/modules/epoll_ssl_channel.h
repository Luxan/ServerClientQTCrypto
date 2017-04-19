#pragma once

#include <QtNetwork>
#include "../interfaces/interface_tcpchannel.h"

class EpollChannel : public InterfaceTcpChannel
{
public:

    EpollChannel(ThreadConfiguration conf, int portNumb, int _maxEvents, Buffer * certificate, Buffer * privateKey);

    virtual ~EpollChannel();
private:
    virtual bool initialize();
    virtual int doListen();
    virtual bool isSocketValid(int i);
    virtual void closeSocket(int i);
    virtual bool checkIncomingConnections(int i);
    int create_and_bind(int port);
    int make_socket_non_blocking(int sfd);
    virtual void handleNotificationOnListningSocket();
    bool readBuffer(QSslSocket *socket, QByteArray &buf);
    bool sendBuffer(uint8_t *buff, size_t size, int i);
    virtual void processPacketExchange(int i);

    QSslSocket *getSslSocket(int i);

    const QSslKey * privateKey;
    const QSslCertificate * certificate;

    struct epoll_event event;
    int sfd;
    int efd;
    struct epoll_event *events;
};
