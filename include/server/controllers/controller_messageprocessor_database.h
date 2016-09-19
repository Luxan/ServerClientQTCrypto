#pragma once

#include "../interfaces/interface_communication_controller.h"
#include "../modules/database.h"
#include "../modules/message_collector.h"
#include "../modules/message_processor.h"

/**
\class
\brief
*/
class Controller_MessageProcessor_Database : public InterfaceCommunicationController
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
    void setDataBaseObj(DataBase * module);
};

