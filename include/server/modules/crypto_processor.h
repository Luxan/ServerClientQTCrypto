#pragma once

#include "../interfaces/interface_processor.h"
#include "../../shared/packages/package.h"
#include "../systemevents.h"
#include "../../../include/server/controllers/controller_decryptionprocessor_threadworker.h"
#include "../../../include/server/controllers/controller_encryptionprocessor_threadworker.h"

class Controller_DecryptionProcessor_ThreadWorker;
class Controller_EncryptionProcessor_ThreadWorker;

class CryptoProcessor : public Processor
{
protected:
    eSystemEvent errorEvent;
    eSystemEvent requestStartWorkerEvent;
    eSystemEvent requestSleepWorkerEvent;
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    CryptoProcessor(ThreadConfiguration &conf,
                    InterfaceCommunicationController *controller,
                    int numberOfWorkers,
                    eSystemEvent errorEvent,
                    eSystemEvent requestStartWorkerEvent,
                    eSystemEvent requestSleepWorkerEvent);


    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void RequestStart();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void RequestStop();

    virtual ~CryptoProcessor(){}
};

class EncryptionProcessor : public CryptoProcessor
{
    /**
    \see interface_thread.h
    */
    virtual void dowork();
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    EncryptionProcessor(ThreadConfiguration conf, Controller_EncryptionProcessor_ThreadWorker *controller, int numberOfWorkers);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void EncryptPackage(PackageWrapper *m);

    virtual ~EncryptionProcessor(){}
};

class DecryptionProcessor : public CryptoProcessor
{
    /**
    \see interface_thread.h
    */
    virtual void dowork();
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    DecryptionProcessor(ThreadConfiguration conf, Controller_DecryptionProcessor_ThreadWorker *controller, int numberOfWorkers);

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void DecryptPackage(PackageWrapper *m);

    virtual ~DecryptionProcessor(){}
};
