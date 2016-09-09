/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef IMULTYMOLDULECOMMUNICATION_H
#define IMULTYMOLDULECOMMUNICATION_H

#include <queue>
#include <mutex>
#include "../impulse.h"
#include "interface_communication_controller.h"

class Impulse;

/**
\class
\brief
*/
class interfaceCommunication
{
private:
    std::mutex lock_queue;
    std::vector<Impulse *> vImpulses;
    std::vector<interfaceCommunicationController *> vControllers;
protected:
    /**
    \threadsafe is thread safe!
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void AddImpulseToQueue(Impulse *i);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void CheckAllEvents();
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void AddEventController(interfaceCommunicationController *c);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    Impulse * getNextImpulse(Impulse * i);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    Impulse * eraseImpulseAndGetNext(Impulse * i);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual ~interfaceCommunication();
};

#endif // IMULTYMOLDULECOMMUNICATION_H
