/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../../include/server/controllers/controller_messagecollector_messageprocessor.h"
#include "../../../include/server/modules/message_collector.h"
#include "../../../include/server/modules/message_processor.h"
#include "../../../include/server/impulse.h"

#include "../../../include/shared/messages/message.h"
#include "../../include/server/login_server/server_logger.h"

void Controller_MessageCollector_MessageProcessor::CheckModule1Events(void *module1, void *nmodule2)
{
    MessageCollector *messageCollector = (MessageCollector *)module1;
    MessageProcessor *eventGiver = (MessageProcessor *)nmodule2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        Message *m;
        switch (i->getEvent())
        {
        case eSystemEvent::MessageProcessed:
            m = ((ImpulseMessage *)i)->getData();
            messageCollector->storeMessageToSend((Message *)m);
            deleteAndNext = true;
            break;
        case eSystemEvent::MessageKeyAgreementProcessed:
            m = ((ImpulseMessage *)i)->getData();
            messageCollector->storeKeyAgreementMessageToSend((MessageSessionDetailResponse *)m);
            deleteAndNext = true;
            break;

        case eSystemEvent::Undefined:
            LOG_ERROR("Got Undefined event!");
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

void Controller_MessageCollector_MessageProcessor::CheckModule2Events(void *module1, void *module2)
{
    MessageProcessor *messageProcessor = (MessageProcessor *)module1;
    MessageCollector *eventGiver = (MessageCollector *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        Message *m;
        switch (i->getEvent())
        {
        case eSystemEvent::MessageCollected:
            m = ((ImpulseMessage *)i)->getData();
            messageProcessor->AddMessageToProcess((MessageProcessable *)m);
            deleteAndNext = true;
            break;
        case eSystemEvent::AttemptToLogIn:
            m = ((ImpulseMessage *)i)->getData();
            messageProcessor->AddMessageToProcess((MessageProcessable *)m);
            deleteAndNext = true;
            break;
        case eSystemEvent::MessageKeyAgreementCollected:
            m = ((ImpulseMessage *)i)->getData();
            messageProcessor->AddKeyAgreementMessageToProcess((MessageSessionDetailRequest *)m);
            deleteAndNext = true;
            break;
        case eSystemEvent::Undefined:
            LOG_ERROR("Got Undefined event!");
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

void Controller_MessageCollector_MessageProcessor::setMessageCollectorObj(MessageCollector * module)
{
    setModule1Obj(module);
}

void Controller_MessageCollector_MessageProcessor::setMessageProcessorObj(MessageProcessor * module)
{
    setModule2Obj(module);
}
