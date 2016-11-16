/**
\author Sergey Gorokh (ESEGORO)
*/
#include <thread>
#include <list>


#include "../../../include/server/systemevents.h"
#include "../../../include/server/impulse.h"
#include "../../../include/server/modules/message_collector.h"
#include "../../../include/server/modules/database.h"
#include "../../../include/shared/packages/package_instant_replay.h"
#include "../../include/server/login_server/server_logger.h"


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
    PackageWrapperDecoded *pw;
    MessageMulty * mm;
    MessageSingle * ms;
    if (mm = dynamic_cast<MessageMulty*>(m))
    {
        std::list<Package *> * list = mm->extractPackages();
        for (Package * package : *list)
        {
            pw = new PackageWrapperDecoded();
            pw->package = package;
            pw->type = mm->getPackageType();
            pw->sessionID = mm->getSessionID();
            AddImpulseToQueue(new ImpulsePackage(eSystemEvent::EncryptPackage, pw));
        }
    }
    if (ms = dynamic_cast<MessageSingle*>(m))
    {
        pw = new PackageWrapperDecoded();
        pw->package = ms->extractPackage();
        pw->type = ms->getPackageType();
        pw->sessionID = ms->getSessionID();
        AddImpulseToQueue(new ImpulsePackage(eSystemEvent::EncryptPackage, pw));
    }
    delete m;
}

void MessageCollector::storeKeyAgreementMessageToSend(MessageSessionDetailResponse *m)
{
    PackageWrapperDecoded *pw = new PackageWrapperDecoded();
    pw->package = m->extractPackage();
    pw->type = m->getPackageType();
    pw->sessionID = m->getSessionID();
    AddImpulseToQueue(new ImpulseEnryptPackage(eSystemEvent::EncryptPackageWithRSA, pw, m->getRsaPublicKey()));
    delete m;
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

void MessageCollector::collectPackage(PackageWrapperDecoded *p)
{
    static std::map<SessionID, std::list<MessageProcessable *>*>::iterator iter;// = qMessages.find(p->sessionID);

    //if (iter == qMessages.end())
    //{
    //    return;
    //}

    Message * m;

    //std::list<MessageProcessable *>* messageList = iter->second;

    switch (p->type) //change to if -> performance
    {
    case PackageWrapper::ePackageType::SessionDetailRequest:
        m = new MessageSessionDetailRequest((PackageSessionDetailRequest*)p->package);
        AddImpulseToQueue(new ImpulseMessage(eSystemEvent::MessageCollected, m));
        break;

    case PackageWrapper::ePackageType::Ping:
        if (((PackagePing*)p->package)->login->getLength() == 0)
        {
            delete p;
            return;
        }
        AddImpulseToQueue(new ImpulseMessage(eSystemEvent::AttemptToLogIn, new MessagePing((PackagePing*)p->package)));
        break;
//        for (std::list<MessageProcessable *>::iterator iter = messageList->begin(); iter != messageList->end(); iter++)
//        {

//        }

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
    //default:
        LOG_ERROR("Undefined package type!");
    }
    delete p;
}
