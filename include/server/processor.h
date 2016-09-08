#pragma once

#include "interface_thread.h"
#include "thread_worker.h"

#include "../server/interface_communication_controller.h"
#include "../../include/server/systemevents.h"
#include "../../include/server/impulse.h"

class Processor : public interfaceThread
{
private:
    int numberOfWorkers;
    std::list<ThreadWorker *> lWorkers;
    TaskQueue tq;
protected:
    interfaceCommunicationController *controller;

    void sendEventToWorkers(eSystemEvent e)
    {
        for (ThreadWorker *w : lWorkers)
        {
            AddImpulseToQueue(new ImpulseSignal(e, w->getThreadID()));
        }
    }

    Processor(ThreadConfiguration &conf):
        interfaceThread(conf)
    {

    }

    virtual ~Processor()
    {
        for (ThreadWorker *w : lWorkers)
        {
            delete w;
        }
    }

    void CreateWorkers(ThreadConfiguration conf, int _numberOfWorkers)
    {
        numberOfWorkers = _numberOfWorkers;
        controller->setModule1Obj(this);
        this->AddEventController(controller);

        for (int i = 0; i < numberOfWorkers; i++)
        {
            ThreadWorker *worker = new ThreadWorker(conf, &tq);

            controller->setModule2Obj(worker);
            worker->AddEventController(controller);

            lWorkers.push_back(worker);
        }
    }
public:
    /**
    \threadsafe using threadsafe TaskQueue tq
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void AddTask(Task * task)
    {
        tq.AddTask(task);
    }
};
