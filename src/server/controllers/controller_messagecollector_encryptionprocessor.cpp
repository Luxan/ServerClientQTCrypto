#include "../../include/server/interfaces/interface_communication_controller.h"
#include "../../include/server/controllers/controller_messagecollector_encryptionprocessor.h"
#include "../../include/server/modules/message_collector.h"
#include "../../include/server/modules/crypto_processor.h"
#include "../../include/server/impulse.h"
#include "../../include/server/slog.h"


void Controller_MessageCollector_EncryptionProcessor::CheckModule1Events(void *module1, void *module2)
{
    EncryptionProcessor *encryptionProcessor = (EncryptionProcessor *)module1;
    MessageCollector *eventGiver = (MessageCollector *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::EncryptPackage:
            encryptionProcessor->EncryptPackage(((ImpulsePackage *)i)->getData());
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

void Controller_MessageCollector_EncryptionProcessor::CheckModule2Events(void *module1, void * module2)
{
    //MessageCollector *collector = (MessageCollector *)module1;
    EncryptionProcessor *eventGiver = (EncryptionProcessor *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        PackageWrapper *p;
        switch (i->getEvent())
        {
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