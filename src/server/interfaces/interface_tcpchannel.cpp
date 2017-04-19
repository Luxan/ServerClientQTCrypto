/**
\author Sergey Gorokh (ESEGORO)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QtNetwork>

#include "../../../include/server/interfaces/interface_tcpchannel.h"
#include "../../../include/shared/messages/message.h"
#include "../../../include/shared/crypto/key.h"
#include "../../../include/shared/user.h"
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
                handleNotificationOnListningSocket();
                continue;
            }
            else
            {
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

void InterfaceTcpChannel::logError(std::string data)
{
    AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorTcpChannel, data));
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
