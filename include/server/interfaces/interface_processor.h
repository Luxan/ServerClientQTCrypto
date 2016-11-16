#pragma once

#include "../interfaces/interface_thread.h"
#include "../thread_worker.h"

#include "../interfaces/interface_communication_controller.h"
#include "../systemevents.h"
#include "../impulse.h"

class Processor : public InterfaceThread
{
private:
    int numberOfWorkers;
    std::list<ThreadWorker *> lWorkers;
    TaskQueue tq;
protected:
    InterfaceCommunicationController *controller;

    void sendEventToWorkers(eSystemEvent e)
    {
        for (ThreadWorker *w : lWorkers)
        {
            AddImpulseToQueue(new ImpulseSignal(e, w->getThreadID()));
        }
    }

    virtual ~Processor()
    {
        terminate();
        for (ThreadWorker *w : lWorkers)
        {
            w->terminate();
            delete w;
        }
        lWorkers.clear();
    }

    ThreadWorker * CreateWorker(ThreadConfiguration conf)
    {
        ThreadWorker *worker = new ThreadWorker(conf, &tq);

        lWorkers.push_back(worker);

        worker->startMainLoop();

        return worker;
    }
public:
    Processor(ThreadConfiguration &conf, InterfaceCommunicationController *controller, int numberOfWorkers):
        InterfaceThread(conf), numberOfWorkers(numberOfWorkers), controller(controller)
    {}
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

    /**
    \threadsafe using threadsafe TaskQueue tq
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    Task * GetNextDoneTask()
    {
        return tq.GetNextDoneTask();
    }

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual void terminate()
    {
        InterfaceThread::terminate();
        for (ThreadWorker *w : lWorkers)
        {
            w->terminate();
        }
    }
};
