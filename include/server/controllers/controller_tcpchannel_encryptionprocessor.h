#pragma once

#include "../interfaces/interface_communication_controller.h"

/**
\class
\brief
*/
class Controller_TCPChannel_EncryptionProcessor : public interfaceCommunicationController
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

