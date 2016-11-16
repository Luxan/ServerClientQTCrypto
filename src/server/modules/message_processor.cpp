#include <thread>

/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../../include/server/task.h"
#include "../../../include/server/modules/message_processor.h"
#include "../../../include/server/thread_worker.h"
#include "../../../include/server/systemevents.h"
#include "../../../include/server/impulse.h"
#include "../../../include/shared/messages/message.h"

MessageProcessor::MessageProcessor(ThreadConfiguration conf, Controller_MessageProcessor_ThreadWorker *controller, int numberOfWorkers, Certificate *certificate):
    Processor(conf, controller, numberOfWorkers), certificate(certificate)
{
    ThreadConfiguration workersConf;
    workersConf.loopSleepTime = 10;
    workersConf.unsleepReactionTime = 1010;
    workersConf.sleepLoopMode = ThreadConfiguration::doSleepInsideLoop;
    workersConf.responseSleepEvent = eSystemEvent::ResponseSleepMessageWorker;
    workersConf.responseStartEvent = eSystemEvent::ResponseStartMessageWorker;

    this->AddEventController(controller);

    controller->setMessageProcessorObj(this);

    for (int i = 0; i < numberOfWorkers; i++)
    {
        ThreadWorker *worker = CreateWorker(workersConf);
        controller->setThreadWorkerObj(worker);
        worker->AddEventController(controller);
    }
}

void MessageProcessor::RequestSleep()
{
    if (!isRunning())
    {
        std::string error = "Server is already stopped.";
        AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorMessageProcessor, error));
        return;
    }

    sendEventToWorkers(eSystemEvent::RequestSleepMessageWorker);

    sleepThread();
}

void MessageProcessor::RequestStart()
{
    if (isRunning())
    {
        std::string error = "Server is already running.";
        AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorMessageProcessor, error));
        return;
    }

    sendEventToWorkers(eSystemEvent::RequestStartMessageWorker);

    unSleepThread();
}

void MessageProcessor::missingMessage(Message * m)
{

}

void MessageProcessor::dowork()
{
    MessageKeyAgreementTask * keyTask;
    MessageTask * task = (MessageTask * )GetNextDoneTask();
    if (task == nullptr)
        return;
    MessageContext * c = (MessageContext*)task->getContext();
    MessageSessionDetailRequest * message = (MessageSessionDetailRequest*)c->extractMessage();
    if (keyTask = dynamic_cast<MessageKeyAgreementTask*>(task))
    {
        MessageKeyAgreementContext * c = (MessageKeyAgreementContext *)task->getContext();
        if (c == nullptr)
        {
            AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorMessageProcessor, "Got done MessageKeyAgreementTask without context!"));
            return;
        }
        PackageSessionDetailResponse * p = new PackageSessionDetailResponse(certificate, c->getAgent()->getAgreedValueLength(), c->getAgent()->getStaticPublicKey(), c->getAgent()->getEphemeralPublicKey());

        AddImpulseToQueue(new ImpulseMessage(eSystemEvent::MessageKeyAgreementProcessed, new MessageSessionDetailResponse(p, message->getRsaPublicKey())));
    }
//    else
//    {
//        CryptoContext * c = (CryptoContext *)task->getContext();

//        AddImpulseToQueue(new ImpulsePackage(eSystemEvent::PackageReceived, c->m));
//    }
}
void MessageProcessor::AddKeyAgreementMessageToProcess(MessageSessionDetailRequest *msg)
{
    std::map<SessionID, KeyAgreamentAgent *>::iterator iter = agentBank.find(msg->getSessionID());

    KeyAgreamentAgent * agent;
    if (iter == agentBank.end())
    {
        agent = new KeyAgreamentAgent(certificate);
    }
    else
    {
        agent = iter->second;
    }
    MessageKeyAgreementContext *c = new MessageKeyAgreementContext(msg, agent);
    MessageKeyAgreementTask *task = new MessageKeyAgreementTask(c);

    AddTask(task);
}

void MessageProcessor::AddMessageToProcess(MessageProcessable *m)
{
    MessageContext *c = new MessageContext(m);
    MessageTask *task = new MessageTask(c);

    AddTask(task);
}

void MessageProcessor::AttemptToLogIn(MessagePing * ping)
{
    AddImpulseToQueue(new ImpulseMessage(eSystemEvent::AttemptToLogIn, ping));
}
