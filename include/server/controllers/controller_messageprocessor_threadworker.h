#pragma once

#include "../interfaces/interface_communication_controller.h"
#include "../thread_worker.h"
#include "../modules/message_processor.h"

class MessageProcessor;
/**
\class
\brief
*/
class Controller_MessageProcessor_ThreadWorker : public InterfaceCommunicationController
{
private:
    /**
    \see interface_communication_controller.h
    */
    void CheckModule1Events(void * module1, void * module2);

    /**
    \see interface_communication_controller.h
    */
    void CheckModule2Events(void * module1, void * module2);
public:
    void setMessageProcessorObj(MessageProcessor * module);
    void setThreadWorkerObj(ThreadWorker * module);
};
