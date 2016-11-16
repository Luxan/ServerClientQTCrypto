#include "../../../include/server/interfaces/interface_communication_controller.h"
#include "../../../include/server/controllers/controller_tcpchannel_decryptionprocessor.h"
#include "../../../include/server/interfaces/interface_tcpchannel.h"
#include "../../../include/server/modules/crypto_processor.h"
#include "../../../include/server/impulse.h"
#include "../../include/server/login_server/server_logger.h"



void Controller_TCPChannel_DecryptionProcessor::CheckModule1Events(void *module1, void *module2)
{
    //InterfaceTcpChannel *tcpChannel = (InterfaceTcpChannel *)module1;
    DecryptionProcessor *eventGiver = (DecryptionProcessor *)module2;
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

void Controller_TCPChannel_DecryptionProcessor::CheckModule2Events(void *module1, void * module2)
{
    DecryptionProcessor *decryptionProcessor = (DecryptionProcessor *)module1;
    InterfaceTcpChannel *eventGiver = (InterfaceTcpChannel *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;
    PackageWrapper *p;
    CryptoTask *task;
    CryptoContext *c;
    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::PackageReceived:
            p = ((ImpulsePackage *)i)->getData();
            decryptionProcessor->DecryptPackage((PackageWrapperEncoded *)p);
            deleteAndNext = true;
            break;
        case eSystemEvent::PackageKeyAgreementReceived:
            p = ((ImpulsePackage *)i)->getData();
            c = new CryptoContext(p, nullptr, false);
            task = new CryptoTask(c);
            decryptionProcessor->AddTask((Task*)task);

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

void Controller_TCPChannel_DecryptionProcessor::setTCPChannelObj(InterfaceTcpChannel * module)
{
    setModule1Obj(module);
}

void Controller_TCPChannel_DecryptionProcessor::setDecryptionProcessorObj(DecryptionProcessor * module)
{
    setModule2Obj(module);
}
