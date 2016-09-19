/**
\author Sergey Gorokh (ESEGORO)
*/
#include <thread>
#include <list>

#include "../../include/server/systemevents.h"
#include "../../include/server/impulse.h"
#include "../../include/server/modules/message_collector.h"
#include "../../include/server/slog.h"
#include "../../include/server/modules/database.h"


MessageCollector::MessageCollector(ThreadConfiguration conf):
    InterfaceThread(conf)
{}

void MessageCollector::RequestStart()
{
    if (isRunning())
    {
        std::string error = "MessageCollector is already running.";
        AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorMessageCollector, error));
        return;
    }

    unSleepThread();
}

void MessageCollector::RequestStop()
{
    if (!isRunning())
    {
        std::string error = "MessageCollector is already stopped.";
        AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorMessageCollector, error));
        return;
    }

    sleepThread();
}

void MessageCollector::storeMessageToSend(Message *m)
{
//    std::shared_ptr<User> u = DataBase::GetDataBase().getUser(m->getReceiverID());

//    if (u != nullptr)
//    {
//        std::list<Package *> &list = m->splitIntoPackages();

//        for (Package *p : list)
//        {
//            PackageWrapper *pw = new PackageWrapper();
//            pw->package = p;
//            pw->type = m->getPackageType();
//            u->pushPackageToSend(pw);
//        }
//    }
//    else
//    {
//        std::string error = "User to send is undefined!";
//        AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorMessageCollector, error));
//    }
}

void MessageCollector::dowork()
{
    //while (!qMessages.empty())
    //{
        //MessageProcessable *m = qMessages.front();
        //qMessages.pop();

//        if (m->isReadyToProcess())
//        {
//            AddImpulseToQueue(new ImpulseMessage(eSystemEvent::MessageCollected, (Message *)m));
//        }
    //}
}

void MessageCollector::collectPackage(PackageWrapper *p)
{
//    switch (p->type) //change to if -> performance
//    {
//    case PackageWrapper::ePackageType::RequestAutocomplete:
//        qMessages.push(new MessageRequestAutocomplete(p->package));
//        break;
//    case PackageWrapper::ePackageType::RequestLeavingRoom:
//        qMessages.push(new MessageRequestLeavingRoom(p->package));
//        break;
//    case PackageWrapper::ePackageType::RequestMessageHistory:
//        qMessages.push(new MessageRequestMessageHistory(p->package));
//        break;
//    case PackageWrapper::ePackageType::RequestJoinRoom:
//        qMessages.push(new MessageRequestJoinRoom(p->package));
//        break;
//    case PackageWrapper::ePackageType::UserMessage:
//        qMessages.push(new MessageUserMessage(p->package));
//        break;
//    case PackageWrapper::ePackageType::UserNotification:
//        qMessages.push(new MessageUserNotification(p->package));
//        break;
//    case PackageWrapper::ePackageType::SetUserInBlackList:
//        qMessages.push(new MessageSetUserInBlackList(p->package));
//        break;
//    case PackageWrapper::ePackageType::UnFriendUser:
//        qMessages.push(new MessageUnFriendUser(p->package));
//        break;
//    case PackageWrapper::ePackageType::Ping:
//        qMessages.push(new MessagePing(p->package));
//        break;
//    case PackageWrapper::ePackageType::Error:
//        qMessages.push(new MessageError(p->package));
//        break;
//    default:
//        SLog::logError() << "Undefined package type!";
//    }
//    delete p;
}
