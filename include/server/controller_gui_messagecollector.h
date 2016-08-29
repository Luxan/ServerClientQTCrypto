/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef CONTROLLER_GUI_MESSAGECONTAINER_H
#define CONTROLLER_GUI_MESSAGECONTAINER_H

#include "../server/interface_communication_controller.h"

/**
\class
\brief
*/
class Controller_GUI_MessageCollector : public interfaceCommunicationController
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

#endif // CONTROLLER_GUI_MESSAGECONTAINER_H
