
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/un.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QtNetwork>

#include "../../../include/server/modules/epoll_ssl_channel.h"
#include "../../../include/shared/buffer_spitter.h"

EpollChannel::EpollChannel(ThreadConfiguration conf, int portNumb, int _maxEvents, Buffer * certificate, Buffer *privateKey):
    InterfaceTcpChannel(conf, portNumb, _maxEvents)
{
    QByteArray baKey((const char *)privateKey->getPointerToBuffer(), privateKey->getLength());
    QByteArray baCertificate((const char *)certificate->getPointerToBuffer(), certificate->getLength());
    QString password = "dupa";
    this->privateKey = new QSslKey(baKey, QSsl::KeyAlgorithm::Rsa, QSsl::Pem, QSsl::PrivateKey, password.toUtf8());
    this->certificate = new QSslCertificate(baCertificate);

    /* Buffer where events are returned */
    events = (epoll_event *)calloc(maxEvents, sizeof event);
}

EpollChannel::~EpollChannel()
{
    delete this->privateKey;
    delete this->certificate;
    free(events);

    if (0 != sfd)
        close(sfd);
}

bool EpollChannel::initialize()
{
    int s;
    try
    {
        sfd = create_and_bind(portno);
        if (sfd == -1)
            throw std::string(" create_and_bind ") + gai_strerror(sfd);

        s = make_socket_non_blocking(sfd);
        if (s == -1)
            throw std::string(" make_socket_non_blocking: ") + gai_strerror(s);

        s = listen(sfd, SOMAXCONN);
        if (s == -1)
            throw std::string(" listen");

        efd = epoll_create1(0);
        if (efd == -1)
            throw std::string(" epoll_create");

        event.data.fd = sfd;
        event.events = EPOLLIN | EPOLLET;
        s = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);
        if (s == -1)
            throw std::string(" epoll_ctl");

        return true;
    }
    catch (std::string e)
    {
        logError(e);

        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestSleepTcpChannel));

        close(sfd);
        return false;
    }
}

int EpollChannel::doListen()
{
    return epoll_wait(efd, events, maxEvents, 1000);
}

bool EpollChannel::isSocketValid(int i)
{
    if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN)))
    {
        /* An error has occured on this fd, or the socket is not
           ready for reading (why were we notified then?) */
        logError("epoll socket error or the socket is not ready for reading!");
        closeSocket(i);
        return false;
    }
    else
        return true;
}

bool EpollChannel::checkIncomingConnections(int i)
{
    if (sfd == events[i].data.fd)
    {
        /* We have a notification on the listening socket, which
           means one or more incoming connections. */
        return true;
    }
    return false;
}

void EpollChannel::handleNotificationOnListningSocket()
{
    int s;
    while (1)
    {
        struct sockaddr in_addr;
        socklen_t in_len;
        int infd;
        char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

        in_len = sizeof in_addr;
        infd = accept(sfd, &in_addr, &in_len);
        if (infd == -1)
        {
            if ((errno == EAGAIN) ||
                    (errno == EWOULDBLOCK))
            {
                /* We have processed all incoming
                   connections. */
                break;
            }
            else
            {
                logError("epol accept error!");
                break;
            }
        }

        s = getnameinfo(&in_addr, in_len,
                        hbuf, sizeof hbuf,
                        sbuf, sizeof sbuf,
                        NI_NUMERICHOST | NI_NUMERICSERV);
        if (s == 0)
        {
            printf("Accepted connection on descriptor %d "
                   "(host=%s, port=%s)\n", infd, hbuf, sbuf);
        }

        /* Make the incoming socket non-blocking and add it to the
           list of fds to monitor. */
        s = make_socket_non_blocking(infd);
        if (s == -1)
            abort();

        event.data.fd = infd;
        event.events = EPOLLIN | EPOLLET;
        s = epoll_ctl(efd, EPOLL_CTL_ADD, infd, &event);
        if (s == -1)
        {
            logError("epoll_ctl");
            abort();
        }
    }
}

bool EpollChannel::sendBuffer(uint8_t *buff, size_t size, int i)
{
    ssize_t s = write(events[i].data.fd, buff, size);

    if (s == -1)
    {
        logError(std::string(" epoll event error: write : ") + gai_strerror(errno));
        return false;
    }
    return true;
}

bool EpollChannel::readBuffer(QSslSocket *socket, QByteArray &buf)
{
    buf.append(socket->readAll());

    if (buf.size() <= 0)
    {
        /* If errno == EAGAIN, that means we have read all
           data. So go back to the main loop. */
        if (errno != EAGAIN)
        {
            logError(std::string(" epoll event error: read!"));
            return false;
        }
        return true;
    }
    else
    {

    }
    return true;
}

void EpollChannel::closeSocket(int i)
{
    close(events[i].data.fd);
}

int EpollChannel::create_and_bind(int port)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
    hints.ai_flags = AI_PASSIVE;     /* All interfaces */

    s = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
    if (s != 0)
    {
        logError(std::string("getaddrinfo: ") + gai_strerror(s));
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
        if (s == 0)
        {
            /* We managed to bind successfully! */
            break;
        }

        close(sfd);
    }

    if (rp == NULL)
    {
        logError("Could not bind.");
        return -1;
    }

    freeaddrinfo(result);

    return sfd;
}

int EpollChannel::make_socket_non_blocking(int sfd)
{
    int flags, s;

    flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1)
    {
        logError("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;

    s = fcntl(sfd, F_SETFL, flags);
    if (s == -1)
    {
        logError("fcntl");
        return -1;
    }

    return 0;
}

QSslSocket * EpollChannel::getSslSocket(int i)
{
    QSslSocket * socket = new QSslSocket();
    socket->setPrivateKey(*privateKey);
    socket->setLocalCertificate(*certificate);
    socket->setSocketDescriptor(events[i].data.fd);
    return socket;
}

void EpollChannel::processPacketExchange(int i)
{
    QSslSocket * socket = getSslSocket(i);
    while (1)
    {
        QByteArray buf;
        if(!readBuffer(socket, buf))
        {
            connectedClients[i].releaseUser();
            socket->close();
            delete socket;
            closeSocket(i);
            break;
        }
        else if (buf.size() == 0)
        {
            /* End of file. The remote has closed the
               connection. */
            connectedClients[i].releaseUser();
            socket->close();
            delete socket;
            closeSocket(i);
            break;
        }

        BufferSpitter b((uint8_t *)buf.data(), buf.size());

        std::list<PackageBuffer *> list;

        b.splitBufferIntoList(list, connectedClients[i].incompletePackageBuffer, connectedClients[i].incompletePackageFullLength);

        processReceivedBuffers(list, i);
    }
}

void EpollChannel::processReceivedBuffers(std::list<PackageBuffer *> &list, int i)
{
    for (PackageBuffer *packageBuff : list)
    {
        PackageWrapperEncoded *pw = CreatePackage(packageBuff);//function delete packageBuff, creates new pw

        if (pw == nullptr)
        {
            continue;
        }

        if (pw->sessionID == 0)
        {
            handleSessionDetailRequestPackage(pw, i);//send pw to other module - dont need to delete pw
            continue;
        }
        else
        {
            AddImpulseToQueue(new ImpulsePackage(eSystemEvent::PackageReceived, pw));//send pw to other module - dont need to delete pw
        }

        std::map<SessionID, std::list<PackageWrapperEncoded*>*>::iterator iter = packagesToSend.find(pw->sessionID);
        if (iter != packagesToSend.end())
        {
            bool error = false;
            for (PackageWrapperEncoded* package : *iter->second)
            {
                if (!sendPackage(package, i))
                {
                    logError("Send package to client failed on socket " + std::to_string(i));
                    error = true;
                    break;
                }
                delete package;
            }
            if (!error)
                iter->second->clear();
        }
    }
}
