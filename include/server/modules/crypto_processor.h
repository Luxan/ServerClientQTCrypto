#pragma once

#include <map>

#include "../interfaces/interface_processor.h"
#include "../../shared/packages/package.h"
#include "../systemevents.h"
#include "../../../include/server/controllers/controller_decryptionprocessor_threadworker.h"
#include "../../../include/server/controllers/controller_encryptionprocessor_threadworker.h"
#include "../../shared/crypto/cipher.h"
#include "../../shared/id_session.h"

class Controller_DecryptionProcessor_ThreadWorker;
class Controller_EncryptionProcessor_ThreadWorker;

class CryptoProcessor : public Processor
{
protected:
    eSystemEvent errorEvent;
    eSystemEvent requestStartWorkerEvent;
    eSystemEvent requestSleepWorkerEvent;
    std::mutex cipherBankLock;
    std::map<SessionID, Cipher*> cipherBank;
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

    virtual ~CryptoProcessor()
    {
        for (std::pair<SessionID, Cipher*> pair : cipherBank)
        {
            delete pair.second;
        }
        cipherBank.clear();
    }
};

class EncryptionProcessor : public CryptoProcessor
{
    /**
    \see interface_thread.h
    */
    virtual void dowork();
    Key * rsaPrivateKey;
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    EncryptionProcessor(ThreadConfiguration conf, Controller_EncryptionProcessor_ThreadWorker *controller, int numberOfWorkers, Key *rsaPrivateKey);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void EncryptPackage(PackageWrapper *m);

    void EncryptPackageWithRSA(PackageWrapper *m, Key *rsaPublicKey);

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
    void DecryptPackage(PackageWrapperEncoded *m);

    virtual ~DecryptionProcessor(){}
};
