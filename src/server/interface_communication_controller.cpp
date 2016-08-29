/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../include/server/interface_communication_controller.h"


void interfaceCommunicationController::setModule1Obj(void *_m1)
{
    module1.push_back(_m1);
}

void interfaceCommunicationController::setModule2Obj(void *_m2)
{
    module2.push_back(_m2);
}

void interfaceCommunicationController::CheckEvents(void *module)
{
    for (void *mod : module1)
    {
        if (module == mod)
        {
            for (void *mod2 : module2)
            {
                CheckModule1Events(module, mod2);
            }
            return;
        }
    }
    for (void *mod : module2)
    {
        if (module == mod)
        {
            for (void *mod1 : module1)
            {
                CheckModule2Events(module, mod1);
            }
            return;
        }
    }
}
