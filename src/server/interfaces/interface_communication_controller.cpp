/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../../include/server/interfaces/interface_communication_controller.h"


void InterfaceCommunicationController::setModule1Obj(void *_m1)
{
    module1.push_back(_m1);
}

void InterfaceCommunicationController::setModule2Obj(void *_m2)
{
    module2.push_back(_m2);
}

void InterfaceCommunicationController::CheckEvents(void *module)
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

void InterfaceCommunicationController::detach(void *module)
{
    for (std::list<void *>::iterator iter = module1.begin(); iter != module1.end(); iter++)
    {
        if (module == *iter)
        {
            module1.erase(iter);
            return;
        }
    }
    for (std::list<void *>::iterator iter = module2.begin(); iter != module2.end(); iter++)
    {
        if (module == *iter)
        {
            module2.erase(iter);
            return;
        }
    }
}
