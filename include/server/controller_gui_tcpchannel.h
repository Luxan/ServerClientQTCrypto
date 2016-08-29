/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef GUI_TCPCHANNEL_CONTROLLER_H
#define GUI_TCPCHANNEL_CONTROLLER_H

#include "../server/interface_communication_controller.h"

/**
\class
\brief
*/
class Controller_GUI_tcpChannel : public interfaceCommunicationController
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
};

#endif // GUI_TCPCHANNEL_CONTROLLER_H
