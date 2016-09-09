#pragma once

#include "interfaces/interface_tcpchannel.h"

class EpollTCPChannel : public interfaceTcpChannel
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
    EpollTCPChannel(ThreadConfiguration conf, int portNumb, int _maxEvents);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual ~EpollTCPChannel();
private:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual bool initialize();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual int doListen();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual bool isSocketValid(int i);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual void closeSocket(int i);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual bool checkIncomingConnections(int i);
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
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual void handleNotificationOnListningSocket();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual bool readBuffer(uint8_t *buf, ssize_t aviableToRead, ssize_t &redData, int i);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual bool sendBuffer(uint8_t *buff, size_t size, int i);

    struct epoll_event event;
    int sfd;
    int efd;
    struct epoll_event *events;
};
