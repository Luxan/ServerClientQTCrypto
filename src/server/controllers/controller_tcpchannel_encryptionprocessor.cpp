#include "../../../include/server/interfaces/interface_communication_controller.h"
#include "../../../include/server/controllers/controller_tcpchannel_encryptionprocessor.h"
#include "../../../include/server/interfaces/interface_tcpchannel.h"
#include "../../../include/server/modules/crypto_processor.h"
#include "../../../include/server/impulse.h"
#include "../../../include/server/slog.h"


void Controller_TCPChannel_EncryptionProcessor::CheckModule1Events(void *module1, void *module2)
{
    InterfaceTcpChannel *tcpChannel = (InterfaceTcpChannel *)module1;
    EncryptionProcessor *eventGiver = (EncryptionProcessor *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;
    PackageWrapper *p;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::PackageToSend:
            p = ((ImpulsePackage *)i)->getData();
            tcpChannel->prepareToSend(p);
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

void Controller_TCPChannel_EncryptionProcessor::CheckModule2Events(void *module1, void * module2)
{
    //DecryptionProcessor *decryptionProcessor = (DecryptionProcessor *)module1;
    InterfaceTcpChannel *eventGiver = (InterfaceTcpChannel *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
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

void Controller_TCPChannel_EncryptionProcessor::setTCPChannelObj(InterfaceTcpChannel * module)
{
    setModule1Obj(module);
}

void Controller_TCPChannel_EncryptionProcessor::setEncryptionProcessorObj(EncryptionProcessor * module)
{
    setModule2Obj(module);
}
