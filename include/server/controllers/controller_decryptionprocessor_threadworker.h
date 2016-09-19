#pragma once

#include "../interfaces/interface_communication_controller.h"
#include "../modules/crypto_processor.h"
#include "../thread_worker.h"

class DecryptionProcessor;

/**
\class
\brief
*/
class Controller_DecryptionProcessor_ThreadWorker : public InterfaceCommunicationController
{
private:
    /**
    \see interface_communication_controller.h
    */
    void CheckModule1Events(void * module1, void * nmodule2);
    /**
    \see interface_communication_controller.h
    */
    void CheckModule2Events(void * module1, void * module2);
public:
    void setDecryptionProcessorObj(DecryptionProcessor * module);
    void setThreadWorkerObj(ThreadWorker * module);
};

