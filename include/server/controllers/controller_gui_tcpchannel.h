/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include "../interfaces/interface_communication_controller.h"

#include "../interfaces/interface_tcpchannel.h"
#include "../mainwindow.h"
/**
\class
\brief
*/
class Controller_GUI_tcpChannel : public InterfaceCommunicationController
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
    void setTCPChannelObj(InterfaceTcpChannel * module);
};
