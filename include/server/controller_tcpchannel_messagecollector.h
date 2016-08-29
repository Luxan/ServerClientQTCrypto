/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef CONTROLLER_TCPCHANNEL_MESSAGECOLLECTOR_H
#define CONTROLLER_TCPCHANNEL_MESSAGECOLLECTOR_H

#include "../server/interface_communication_controller.h"

/**
\class
\brief
*/
class Controller_tcpChannel_MessageCollector : public interfaceCommunicationController
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

#endif // CONTROLLER_TCPCHANNEL_MESSAGECOLLECTOR_H
