#include "../../include/server/interfaces/interface_communication.h"

void interfaceCommunication::AddImpulseToQueue(Impulse *i)
{
    std::lock_guard<std::mutex> guard(lock_queue);
    vImpulses.push_back(i);
}

void interfaceCommunication::CheckAllEvents()
{
    for (InterfaceCommunicationController *c : vControllers)
    {
        c->CheckEvents(this);
    }
}

void interfaceCommunication::AddEventController(InterfaceCommunicationController *c)
{
    vControllers.push_back(c);
}

Impulse *interfaceCommunication::getNextImpulse(Impulse *i)
{
    std::lock_guard<std::mutex> guard(lock_queue);
    if (i == nullptr)
    {
        if (vImpulses.empty())
            return nullptr;

        return *(vImpulses.begin());
    }

    i->setProcessingState(false);

    bool found = false;

    for (std::vector<Impulse *>::iterator it = vImpulses.begin(); it != vImpulses.end(); it++)
    {
        if (!found)
        {
            if (i == *it)
            {
                found = true;
            }

            continue;
        }

        if ((*it)->isInProcessingState())
        {
            continue;
        }

        (*it)->setProcessingState(true);
        return *it;
    }

    return nullptr;
}


Impulse *interfaceCommunication::eraseImpulseAndGetNext(Impulse *i)
{
    std::lock_guard<std::mutex> guard(lock_queue);

    if (i == nullptr)
    {
        return nullptr;
    }

    bool erase = true;

    i->setProcessingState(false);

    for (std::vector<Impulse *>::iterator it = vImpulses.begin(); it != vImpulses.end(); it++)
    {
        if (erase)
        {
            if (i == *it)
            {
                it = vImpulses.erase(it);
                erase = false;
            }
            else
            {
                continue;
            }
        }

        if (it == vImpulses.end())
        {
            return nullptr;
        }

        Impulse * i = *it;

        if (i == nullptr)
        {
            break;
        }

        if (i->isInProcessingState())
        {
            continue;
        }


        (*it)->setProcessingState(true);
        return *it;
    }

    return nullptr;
}

interfaceCommunication::~interfaceCommunication()
{
    std::lock_guard<std::mutex> guard(lock_queue);

    for (Impulse *i : vImpulses)
    {
        delete i;
    }
    vImpulses.clear();
}
