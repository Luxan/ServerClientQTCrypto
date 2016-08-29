/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../include/server/controller_tcpchannel_messagecollector.h"
#include "../../include/server/tcpchannel.h"
#include "../../include/server/message_collector.h"
#include "../../include/server/impulse.h"
#include "../../include/server/slog.h"

void Controller_tcpChannel_MessageCollector::CheckModule1Events(void *module1, void *module2)
{
    //TcpChannel *tcpChannel = (TcpChannel *)module1;
    MessageCollector *eventGiver = (MessageCollector *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::ErrorTcpChannel:
            SLog::logError() << "MessageCollector Error: " + ((ImpulseError *)i)->getError();
            deleteAndNext = true;
            break;
        case eSystemEvent::Undefined:
            SLog::logError() << "Got Undefined event!";
            deleteAndNext = true;
            break;
        default:
            break;
        }

        if (deleteAndNext)
        {
            todelete = i;
            i = eventGiver->eraseImpulseAndGetNext(i);
            delete todelete;
            deleteAndNext = false;
        }
        else
        {
            i = eventGiver->getNextImpulse(i);
        }
    }
}

void Controller_tcpChannel_MessageCollector::CheckModule2Events(void *module1, void * module2)
{
    MessageCollector *tcpChannel = (MessageCollector *)module1;
    TcpChannel *eventGiver = (TcpChannel *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        PackageWrapper *p;
        switch (i->getEvent())
        {
        case eSystemEvent::PackageReceived:
            p = ((ImpulsePackage *)i)->getData();
            tcpChannel->CollectPackage(p);
            deleteAndNext = true;
            break;
        case eSystemEvent::ErrorMessageCollector:
            SLog::logError() << "tcpChannel Error: " << ((ImpulseError *)i)->getError();
            deleteAndNext = true;
            break;
        case eSystemEvent::Undefined:
            SLog::logError() << "Got Undefined event!";
            deleteAndNext = true;
            break;
        default:
            break;
        }

        if (deleteAndNext)
        {
            todelete = i;
            i = eventGiver->eraseImpulseAndGetNext(i);
            delete todelete;
            deleteAndNext = false;
        }
        else
        {
            i = eventGiver->getNextImpulse(i);
        }
    }
}
