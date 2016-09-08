#pragma once

#include "processor.h"

class CryptoProcessor : public Processor
{
protected:
    eSystemEvent errorEvent;
    eSystemEvent requestStartWorkerEvent;
    eSystemEvent requestSleepWorkerEvent;
    /**
    \see interface_thread.h
    */
    void dowork();
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
                    int numberOfWorkers,
                    eSystemEvent errorEvent,
                    eSystemEvent requestStartWorkerEvent,
                    eSystemEvent requestSleepWorkerEvent,
                    eSystemEvent responseSleepWorkerEvent,
                    eSystemEvent responseStartWorkerEvent);


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
    void RequestSleep();
};

class EncryptionProcessor : public CryptoProcessor
{
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    EncryptionProcessor(ThreadConfiguration conf, int numberOfWorkers):
        CryptoProcessor(conf,
                        numberOfWorkers,
                        eSystemEvent::ErrorEncryptionProcessor,
                        eSystemEvent::RequestStartEncryptionWorker,
                        eSystemEvent::RequestSleepEncryptionWorker,
                        eSystemEvent::ResponseStartEncryptionWorker,
                        eSystemEvent::ResponseSleepEncryptionWorker)
    {}    
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void EncryptPackage(PackageBuffer *m);
    /**
    \see interface_thread.h
    */
    void dowork();
};

class DecryptionProcessor : public CryptoProcessor
{
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    DecryptionProcessor(ThreadConfiguration conf, int numberOfWorkers):
        CryptoProcessor(conf,
                        numberOfWorkers,
                        eSystemEvent::ErrorDencryptionProcessor,
                        eSystemEvent::RequestStartDencryptionWorker,
                        eSystemEvent::RequestSleepDencryptionWorker,
                        eSystemEvent::ResponseStartDencryptionWorker,
                        eSystemEvent::ResponseSleepDencryptionWorker)
    {}

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void DecryptPackage(PackageBuffer *m);
    /**
    \see interface_thread.h
    */
    void dowork();
};
