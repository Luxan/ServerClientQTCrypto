#include "../../../include/server/interfaces/interface_communication_controller.h"
#include "../../../include/server/controllers/controller_messagecollector_decryptionprocessor.h"
#include "../../../include/server/modules/message_collector.h"
#include "../../../include/server/modules/crypto_processor.h"
#include "../../../include/server/impulse.h"
#include "../../include/server/login_server/server_logger.h"



void Controller_MessageCollector_DecryptionProcessor::CheckModule1Events(void *module1, void *module2)
{
    //DecryptionProcessor *decryptionProcessor = (DecryptionProcessor *)module1;
    MessageCollector *eventGiver = (MessageCollector *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
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

void Controller_MessageCollector_DecryptionProcessor::CheckModule2Events(void *module1, void * module2)
{
    MessageCollector *collector = (MessageCollector *)module1;
    DecryptionProcessor *eventGiver = (DecryptionProcessor *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;
    PackageWrapper *p;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::PackageReceived:
            p = ((ImpulsePackage *)i)->getData();
            collector->collectPackage((PackageWrapperDecoded*)p);
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

void Controller_MessageCollector_DecryptionProcessor::setMessageCollectorObj(MessageCollector * module)
{
    setModule1Obj(module);
}

void Controller_MessageCollector_DecryptionProcessor::setDecryptionProcessorObj(DecryptionProcessor * module)
{
    setModule2Obj(module);
}
