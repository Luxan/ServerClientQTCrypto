/**
\author Sergey Gorokh (ESEGORO)
*/
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/un.h>
#include <stdlib.h>

#include <string.h>

#include <thread>
#include <fcntl.h>

#include "../../include/server/tcpchannel.h"
#include "../../include/shared/message.h"
#include "../../include/server/slog.h"
#include "../../include/shared/key.h"
#include "../../include/shared/cipher.h"
#include "../../include/shared/user.h"
#include "../../include/server/database.h"
#include "../../include/shared/buffer.h"
#include "../../include/shared/package_instant_replay.h"
#include "../../include/shared/package_signal.h"
#include "../../include/shared/package_information.h"
#include "../../include/shared/package_update.h"
#include "../../include/shared/buffer_spitter.h"
#include "../../include/shared/error_enum.h"

#include <QDebug>

TcpChannel::TcpChannel(ThreadConfiguration conf, const int portNumb, int _maxEvents):
    interfaceThread(conf),
    maxEvents(_maxEvents),
    portno(portNumb)
{
    /* Buffer where events are returned */
    events = (epoll_event *)calloc(maxEvents, sizeof event);
    connectedClients = new ClientSocket[maxEvents];
    state = UnInitialized;
}

TcpChannel::~TcpChannel()
{
    free(events);

    delete[] connectedClients;
    if (0 != sfd)
        close(sfd);
}

void TcpChannel::setCipher(Hasher *hasher, Cipher * cipher)
{
    this->hasher = hasher;
    this->cipher = cipher;
}

void TcpChannel::RequestStart()
{
    if (isRunning())
    {
        LogError("Server is already running.");

        return;
    }

    unSleepThread();
}

void TcpChannel::RequestStop()
{
    if (!isRunning())
    {
        LogError("Server is already stopped.");

        return;
    }

    sleepThread();
}

bool TcpChannel::initialize(struct epoll_event &event)
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
        LogError(e);

        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestSleepTcpChannel));

        close(sfd);
        return false;
    }
}

void TcpChannel::dowork()
{
    switch (state)
    {
    case UnInitialized:
        if (!initialize(event))
        {
            sleepThread();
            state = UnInitialized;
            return;
        }
        state = Listning;
        break;
    case Listning:
        int n, i;

        n = epoll_wait(efd, events, maxEvents, -1);
        for (i = 0; i < n; i++)
        {
            if ((events[i].events & EPOLLERR) ||
                    (events[i].events & EPOLLHUP) ||
                    (!(events[i].events & EPOLLIN)))
            {
                /* An error has occured on this fd, or the socket is not
                   ready for reading (why were we notified then?) */
                fprintf(stderr, "epoll error\n");
                close(events[i].data.fd);
                continue;
            }

            else if (sfd == events[i].data.fd)
            {
                /* We have a notification on the listening socket, which
                   means one or more incoming connections. */
                handleNotificationOnListningSocket();
                continue;
            }
            else
            {
                /* We have data on the fd waiting to be read. Read and
                   display it. We must read whatever data is available
                   completely, as we are running in edge-triggered mode
                   and won't get a notification again for the same
                   data. */
                processPacketExchange(i);
            }
        }
        break;
    }
}

void TcpChannel::handleNotificationOnListningSocket()
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
                perror("accept");
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
            perror("epoll_ctl");
            abort();
        }
    }
}

bool TcpChannel::sendBuffer(uint8_t *buff, size_t size, int i)
{
    ssize_t s = write(events[i].data.fd, buff, size);

    if (s == -1)
    {
        LogError(std::string(" epoll event error: write : ") + gai_strerror(errno));
        return false;
    }
    return true;
}

bool TcpChannel::sendPackageMultyMessage(PackageWrapper::ePackageType type, PackageMultiPackage *mp, int i)
{
    uint8_t size = sizeof(type) + sizeof(mp->multiPackSize) +
                   sizeof(mp->multiPackCurrentSize) + mp->buff->getLength();

    if (!sendBuffer(&size, sizeof(size), i))
        return false;

    if (!sendBuffer((uint8_t *)&type, sizeof(type), i))
        return false;

    if (!sendBuffer(&mp->multiPackSize, sizeof(mp->multiPackSize), i))
        return false;

    if (!sendBuffer(&mp->multiPackCurrentSize, sizeof(mp->multiPackCurrentSize), i))
        return false;

    if (!sendBuffer(mp->buff->getPointerToBuffer(), mp->buff->getLength(), i))
        return false;

    return true;
}

bool TcpChannel::sendStrictSizePackage(PackageStrictSize * sp, uint8_t strictSize, PackageWrapper::ePackageType type, int i)
{
    uint8_t size = (uint8_t) strictSize + sizeof(type);
    if (!sendBuffer((uint8_t *)&size, sizeof(size), i))
        return false;

    if (!sendBuffer((uint8_t *)&type, sizeof(type), i))
        return false;

    if (!sendBuffer((uint8_t *)sp, strictSize, i))
        return false;

    return true;
}

bool TcpChannel::sendPackage(PackageWrapper *response, int i)
{
    uint8_t strictSize;
    switch (response->type)
    {
//dynamic size packages
    case PackageWrapper::ePackageType::ResponseUserDetails:
    case PackageWrapper::ePackageType::ResponseAutocomplete:
    case PackageWrapper::ePackageType::UpdateBlackList:
    case PackageWrapper::ePackageType::UpdateFriendStatus:
    case PackageWrapper::ePackageType::UpdateRoomPresence:
    case PackageWrapper::ePackageType::UpdateBlackListPresence:
    case PackageWrapper::ePackageType::UserMessage:
    case PackageWrapper::ePackageType::UserNotification:
        if (!sendPackageMultyMessage(response->type, (PackageMultiPackage *)response->package, i))
            return false;
        break;
//strict size packages
    case PackageWrapper::ePackageType::Ping:
        strictSize = ((PackagePing *) response->package)->strictSize();
        if (!sendStrictSizePackage((PackageStrictSize *) response->package, strictSize, response->type, i))
            return false;
    case PackageWrapper::ePackageType::ResponseLogin:
        strictSize = ((PackageResponseLogin *) response->package)->strictSize();
        if (!sendStrictSizePackage((PackageStrictSize *) response->package, strictSize, response->type, i))
            return false;
    case PackageWrapper::ePackageType::ResponsePublicKey:
        strictSize = ((PackageResponsePublicKey *) response->package)->strictSize();
        if (!sendStrictSizePackage((PackageStrictSize *) response->package, strictSize, response->type, i))
            return false;
    case PackageWrapper::ePackageType::Error:
        strictSize = ((PackageError *) response->package)->strictSize();
        if (!sendStrictSizePackage((PackageStrictSize *) response->package, strictSize, response->type, i))
            return false;
        break;
// packages that should never be sent by server
    case PackageWrapper::ePackageType::RequestUserDetails:
    case PackageWrapper::ePackageType::RequestAutocomplete:
    case PackageWrapper::ePackageType::RequestJoinRoom:
    case PackageWrapper::ePackageType::RequestLogin:
    case PackageWrapper::ePackageType::UnFriendUser:
    case PackageWrapper::ePackageType::RequestPublicKey:
    case PackageWrapper::ePackageType::RequestLeavingRoom:
    case PackageWrapper::ePackageType::RequestMessageHistory:
    case PackageWrapper::ePackageType::SetUserInBlackList:
        SLog::logError() << "Incorrect package to send!";
        break;
    }
    return true;
}

void TcpChannel::handleLoginPackage(Package *p, int i)
{
    std::string login = (const char *)((PackageRequestLogin *)p)->login;
    std::string password = (const char *)((PackageRequestLogin *)p)->password;

    std::shared_ptr<User> u = DataBase::GetDataBase().getUser(login, password);

    PackageWrapper response;

    if (u)
    {
        connectedClients[i].connectUser(u);

        response.package = new PackageResponseLogin(u->getID());
        response.type = PackageWrapper::ePackageType::ResponseLogin;

        if (!sendPackage(&response, i))
        {
            delete response.package;
            return;
        }

        delete response.package;

        //SendLoginPackageSequense(u);
    }
    else
    {
        response.package = new PackageError(eError::WrongLoginOrPassword);
        response.type = PackageWrapper::ePackageType::Error;

        if (!sendPackage(&response, i))
        {
            delete response.package;
            return;
        }

        delete response.package;
    }
}

void TcpChannel::handleKeyRequestPackage(int i)
{
    //PackageWrapper response;
    //response.package = new PackageResponsePublicKey(new PackageBuffer(cipher->getPublicKey()->getBuff(), KEY_LENGTH));

    //if (!sendPackage(&response, i))
    //{
    //    delete response.package;
    //    return;
    //}

    //delete response.package;
}

void TcpChannel::processPacketExchange(int i)
{
    int done = 0;

    while (1)
    {
        ssize_t size;
        char buf[512];

        size = read(events[i].data.fd, buf, sizeof buf);
        if (size == -1)
        {
            /* If errno == EAGAIN, that means we have read all
               data. So go back to the main loop. */
            if (errno != EAGAIN)
            {
                LogError(std::string(" epoll event error: read!"));
                done = 1;
            }
            break;
        }
        else if (size == 0)
        {
            /* End of file. The remote has closed the
               connection. */
            connectedClients[i].releaseUser();
            done = 1;
            break;
        }

        for (int x = 0; x < size; x++)
        {
            qDebug() << QByteArray(std::to_string((uint8_t)buf[x]).c_str(), size);
        }

        bufferSpitter b((uint8_t *)buf, size);

        std::list<PackageBuffer *> list;

        b.splitBufferIntoList(list, connectedClients[i].incompletePackageBuffer, connectedClients[i].incompletePackageFullLength);

        processReceivedBuffers(list, i);
    }

    if (done)
    {
        printf("Closed connection on descriptor %d\n",
               events[i].data.fd);

        /* Closing the descriptor will make epoll remove it
           from the set of descriptors which are monitored. */
        close(events[i].data.fd);
    }
}

void TcpChannel::processReceivedBuffers(std::list<PackageBuffer *> &list, int i)
{
    for (PackageBuffer *packageBuff : list)
    {
        PackageWrapper *pw = CreatePackage(packageBuff);

        if (pw == nullptr)
            return;

        if (pw->type == PackageWrapper::ePackageType::RequestLogin)
        {
            handleLoginPackage(pw->package, i);
            delete pw->package;
            return;
        }
        if (pw->type == PackageWrapper::ePackageType::RequestPublicKey)
        {
            delete pw->package;
            handleKeyRequestPackage(i);
            return;
        }

        ClientID id;

        if (!connectedClients[i].isConnected())
        {
            //PackageProcessable *pp = (PackageProcessable *) pw->package;
            //connectedClients[i].connectUser(DataBase::GetDataBase().getUser(pp->sourseID));
        }
        else
        {
            id = connectedClients[i].getUser()->getID();
        }

        AddImpulseToQueue(new ImpulsePackage(eSystemEvent::PackageReceived, pw));

        if (connectedClients[i].isConnected())
        {
            if (connectedClients[i].getUser()->hasPacketsToSend())
            {
                PackageWrapper *p = connectedClients[i].getUser()->getPackageToSend();
                while (p != nullptr)
                {
                    if (!sendPackage(p, i))
                    {
                        delete p;
                        break;
                    }
                    delete p;
                    connectedClients[i].getUser()->popPackageToSend();
                }
            }
        }
    }
}

PackageWrapper *TcpChannel::CreatePackage(PackageBuffer *buf)
{
    if (buf == nullptr)
        return nullptr;

    PackageWrapper *pw = new PackageWrapper();
    try
    {
        buf->fillBuffer((uint8_t *)&pw->type, sizeof(pw->type));

        switch (pw->type)//change to if -> performance
        {
        case PackageWrapper::ePackageType::RequestAutocomplete:
            pw->package = new PackageRequestAutocomplete(buf);
            break;
        case PackageWrapper::ePackageType::RequestLogin:
            pw->package = new PackageRequestLogin(buf);
            break;
        case PackageWrapper::ePackageType::ResponseUserDetails:
            throw ("why would client send ResponseUserDetails to server?!");
            break;
        case PackageWrapper::ePackageType::ResponseAutocomplete:
            throw ("why would client send ResponseAutocomplete to server?!");
            break;
        case PackageWrapper::ePackageType::ResponsePublicKey:
            throw ("why would client send ResponsePublicKey to server?!");
            break;
        case PackageWrapper::ePackageType::ResponseLogin:
            throw ("why would client send ResponseLogin to server?!");
            break;
        case PackageWrapper::ePackageType::UpdateBlackList:
            throw ("why would client send UpdateBlackList to server?!");
            break;
        case PackageWrapper::ePackageType::UpdateFriendStatus:
            throw ("why would client send UpdateBlackList to server?!");
            break;
        case PackageWrapper::ePackageType::UpdateRoomPresence:
            throw ("why would client send UpdateRoomPresence to server?!");
            break;
        case PackageWrapper::ePackageType::UpdateBlackListPresence:
            throw ("why would client send BlackListPresenceUpdate to server?!");
            break;
        case PackageWrapper::ePackageType::UserMessage:
            pw->package = new PackageUserMessage(buf);
            break;
        case PackageWrapper::ePackageType::UserNotification:
            pw->package = new PackageUserNotification(buf);
            break;
            //strict size

        case PackageWrapper::ePackageType::RequestUserDetails:
            pw->package = new PackageRequestUserDetails();
            memcpy(pw->package, buf->getPointerToBuffer(), sizeof(PackageRequestUserDetails));
            delete buf;
            break;
        case PackageWrapper::ePackageType::RequestLeavingRoom:
            pw->package = new PackageRequestLeavingRoom();
            memcpy(pw->package, buf->getPointerToBuffer(), sizeof(PackageRequestLeavingRoom));
            delete buf;
            break;
        case PackageWrapper::ePackageType::SetUserInBlackList:
            pw->package = new PackageSetUserInBlackList();
            memcpy(pw->package, buf->getPointerToBuffer(), sizeof(PackageSetUserInBlackList));
            delete buf;
            break;
        case PackageWrapper::ePackageType::UnFriendUser:
            pw->package = new PackageUnFriendUser();
            memcpy(pw->package, buf->getPointerToBuffer(), sizeof(PackageUnFriendUser));
            delete buf;
            break;
        case PackageWrapper::ePackageType::RequestPublicKey:
            pw->package = new PackageRequestPublicKey();
            memcpy(pw->package, buf->getPointerToBuffer(), sizeof(PackageRequestPublicKey));
            delete buf;
            break;
        case PackageWrapper::ePackageType::RequestMessageHistory:
            pw->package = new PackageRequestMessageHistory();
            memcpy(pw->package, buf->getPointerToBuffer(), sizeof(PackageRequestMessageHistory));
            delete buf;
            break;
        case PackageWrapper::ePackageType::RequestJoinRoom:
            pw->package = new PackageRequestJoinRoom();
            memcpy(pw->package, buf->getPointerToBuffer(), sizeof(PackageRequestJoinRoom));
            delete buf;
            break;
        case PackageWrapper::ePackageType::Ping:
            pw->package = new PackagePing();
            memcpy(pw->package, buf->getPointerToBuffer(), sizeof(PackagePing));
            delete buf;
            break;
        case PackageWrapper::ePackageType::Error:
            pw->package = new PackageError();
            memcpy(pw->package, buf->getPointerToBuffer(), sizeof(PackageError));
            delete buf;
            break;
        default:
            throw ("Undefined package type!");
        }
        return pw;
    }
    catch (std::string error)
    {
        LogError(error);
        delete buf;
        delete pw;

        return nullptr;
    }
}

void TcpChannel::LogError(std::string data)
{
    AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorTcpChannel, data));
}

int TcpChannel::create_and_bind(int port)
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
        LogError(std::string("getaddrinfo: ") + gai_strerror(s));
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
        LogError("Could not bind.");
        return -1;
    }

    freeaddrinfo(result);

    return sfd;
}

int TcpChannel::make_socket_non_blocking(int sfd)
{
    int flags, s;

    flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1)
    {
        LogError("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;

    s = fcntl(sfd, F_SETFL, flags);
    if (s == -1)
    {
        LogError("fcntl");
        return -1;
    }

    return 0;
}
