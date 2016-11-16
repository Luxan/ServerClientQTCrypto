/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include "../interfaces/interface_communication_controller.h"
#include "../login_server/mainwindow.h"
#include "../modules/message_collector.h"

/**
\class
\brief
*/
class Controller_GUI_MessageCollector : public InterfaceCommunicationController
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
    void setMessageCollectorObj(MessageCollector * module);
};

