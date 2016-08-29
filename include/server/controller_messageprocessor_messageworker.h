#ifndef CONTROLLER_MESSAGEPROCESSOR_MESSAGEWORKER_H
#define CONTROLLER_MESSAGEPROCESSOR_MESSAGEWORKER_H

#include "../server/interface_communication_controller.h"

/**
\class
\brief
*/
class Controller_MessageProcessor_MessageWorker : public interfaceCommunicationController
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

#endif // CONTROLLER_MESSAGEPROCESSOR_MESSAGEWORKER_H
