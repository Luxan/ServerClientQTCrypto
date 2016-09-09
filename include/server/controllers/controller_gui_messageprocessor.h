#pragma once

#include "../interfaces/interface_communication_controller.h"

class Controller_GUI_MessageProcessor : public interfaceCommunicationController
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
