#pragma once

#include "../interfaces/interface_communication_controller.h"
#include "../login_server/mainwindow.h"
#include "../modules/message_processor.h"

class Controller_GUI_MessageProcessor : public InterfaceCommunicationController
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
    void setMainWindowObj(MainWindow * module);
    void setMessageProcessorObj(MessageProcessor * module);
};
