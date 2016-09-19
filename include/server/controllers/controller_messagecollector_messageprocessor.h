/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include "../interfaces/interface_communication_controller.h"
#include "../modules/message_processor.h"
#include "../modules/message_collector.h"

/**
\class
\brief
*/
class Controller_MessageCollector_MessageProcessor : public InterfaceCommunicationController
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
    void setMessageCollectorObj(MessageCollector * module);
    void setMessageProcessorObj(MessageProcessor * module);
};
