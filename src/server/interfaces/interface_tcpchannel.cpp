/**
\author Sergey Gorokh (ESEGORO)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/server/interfaces/interface_tcpchannel.h"
#include "../../../include/shared/messages/message.h"
#include "../../../include/shared/crypto/key.h"
#include "../../../include/shared/crypto/crypto.h"
#include "../../../include/shared/user.h"
#include "../../../include/server/modules/database.h"
#include "../../../include/shared/buffer.h"
#include "../../../include/shared/packages/package_instant_replay.h"
#include "../../../include/shared/packages/package_signal.h"
#include "../../../include/shared/packages/package_user_to_user.h"
#include "../../../include/shared/packages/package_update.h"
#include "../../../include/shared/buffer_spitter.h"
#include "../../../include/shared/error_enum.h"

InterfaceTcpChannel::InterfaceTcpChannel(ThreadConfiguration conf, int portNumb, int maxEvents):
    InterfaceThread(conf),
    maxEvents(maxEvents),
    portno(portNumb)
{
    state = UnInitialized;
    connectedClients = new RemoteClient[maxEvents];
}

InterfaceTcpChannel::~InterfaceTcpChannel()
{
    for (std::pair<SessionID, std::list<PackageWrapperEncoded*>*> pair : packagesToSend)
    {
        for (std::list<PackageWrapperEncoded*>::iterator iter = pair.second->begin(); iter != pair.second->end(); iter++)
        {
            delete *iter;
        }
        delete pair.second;
    }

    delete connectedClients;
}

void InterfaceTcpChannel::prepareToSend(PackageWrapperEncoded * pw)
{
    std::map<SessionID, std::list<PackageWrapperEncoded*>*>::iterator iter = packagesToSend.find(pw->sessionID);
    std::list<PackageWrapperEncoded*>* packageList;
    if (iter == packagesToSend.end())
    {
        packageList = new std::list<PackageWrapperEncoded*>();
        packagesToSend[pw->sessionID] = packageList;
    }
    else
    {
        packageList = iter->second;
    }
    packageList->push_back(pw);
}

void InterfaceTcpChannel::RequestStart()
{
    if (isRunning())
    {
        logError("Server is already running.");

        return;
    }

    unSleepThread();
}

void InterfaceTcpChannel::RequestStop()
{
    if (!isRunning())
    {
        logError("Server is already stopped.");

        return;
    }

    sleepThread();
}

void InterfaceTcpChannel::dowork()
{
    switch (state)
    {
    case UnInitialized:
        if (!initialize())
        {
            sleepThread();
            state = UnInitialized;
            return;
        }
        state = Listning;
        break;
    case Listning:
        int n = doListen();

        for (int i = 0; i < n; i++)
        {
            if (!isSocketValid(i))
            {
                continue;
            }
            else if (checkIncomingConnections(i))
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

bool InterfaceTcpChannel::sendPackage(PackageWrapperEncoded *response, int i)
{
    BUFF_SIZE size = response->buffer->getLength();
    if (!sendBuffer((uint8_t*)&size, sizeof(response->buffer->getLength()), i))
        return false;
    if (!sendBuffer(response->buffer->getPointerToBuffer(), response->buffer->getLength(), i))
        return false;

    return true;
}

void InterfaceTcpChannel::handleSessionDetailRequestPackage(PackageWrapperEncoded * pw, int i)
{
    SessionID id;
    pw->sessionID = id;
    AddImpulseToQueue(new ImpulsePackage(eSystemEvent::PackageReceived, pw));

    PackageWrapperEncoded response;
    PackageSessionID package(id);
    response.buffer = package.toPackageBuffer();
    if (!sendPackage(&response, i))
    {
        return;
    }
}

void InterfaceTcpChannel::processPacketExchange(int i)
{
    while (1)
    {
        static ssize_t size;
        static uint8_t buf[512];

        if(!readBuffer(buf, sizeof buf, size, i))
        {
            connectedClients[i].releaseUser();
            closeSocket(i);
            break;
        }
        else if (size == 0)
        {
            /* End of file. The remote has closed the
               connection. */
            connectedClients[i].releaseUser();
            closeSocket(i);
            break;
        }

        BufferSpitter b((uint8_t *)buf, size);

        std::list<PackageBuffer *> list;

        b.splitBufferIntoList(list, connectedClients[i].incompletePackageBuffer, connectedClients[i].incompletePackageFullLength);

        processReceivedBuffers(list, i);
    }
}

void InterfaceTcpChannel::processReceivedBuffers(std::list<PackageBuffer *> &list, int i)
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
//deleting buffer!
PackageWrapperEncoded *InterfaceTcpChannel::CreatePackage(PackageBuffer *buf)
{
    if (buf == nullptr)
        return nullptr;

    PackageWrapperEncoded *pw = new PackageWrapperEncoded();
    try
    {
        buf->fillBuffer((uint8_t *)&pw->sessionID, sizeof(pw->sessionID));
        pw->buffer = buf;
        return pw;
    }
    catch (std::string error)
    {   
        logError(error);
        delete buf;
        delete pw;

        return nullptr;
    }
}

void InterfaceTcpChannel::logError(std::string data)
{
    AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorTcpChannel, data));
}
