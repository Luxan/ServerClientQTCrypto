/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef SYSTEMEVENTS_H
#define SYSTEMEVENTS_H

#include <stdint.h>

/**
\enum
\brief
*/
enum class eSystemEvent : uint8_t
{
    RequestStartDatabase,
    RequestSleepDatabase,
    ResponseStartDatabase,
    ResponseSleepDatabase,

    RequestStartEncryptionProcessor,
    RequestSleepEncryptionProcessor,
    ResponseStartEncryptionProcessor,
    ResponseSleepEncryptionProcessor,

    RequestStartDecryptionProcessor,
    RequestSleepDecryptionProcessor,
    ResponseStartDecryptionProcessor,
    ResponseSleepDecryptionProcessor,

    RequestStartMessageCollector,
    RequestSleepMessageCollector,
    ResponseStartMessageCollector,
    ResponseSleepMessageCollector,

    RequestStartMessageProcessor,
    RequestSleepMessageProcessor,
    ResponseStartMessageProcessor,
    ResponseSleepMessageProcessor,

    //channel
    RequestStartTcpChannel,
    RequestSleepTcpChannel,
    ResponseStartTcpChannel,
    ResponseSleepTcpChannel,
//workers
    RequestStartMessageWorker,
    RequestSleepMessageWorker,
    ResponseStartMessageWorker,
    ResponseSleepMessageWorker,

    RequestStartEncryptionWorker,
    RequestSleepEncryptionWorker,
    ResponseStartEncryptionWorker,
    ResponseSleepEncryptionWorker,

    RequestStartDencryptionWorker,
    RequestSleepDencryptionWorker,
    ResponseStartDencryptionWorker,
    ResponseSleepDencryptionWorker,

//errors
    ErrorDatabase,
    ErrorEncryptionProcessor,
    ErrorDecryptionProcessor,
    ErrorMessageCollector,
    ErrorTcpChannel,
    ErrorMessageProcessor,
    ErrorGui,
    ErrorMessageWorker,
//data events
    EncryptPackage,
    DecryptPackage,
    PackageReceived,
    PackageToSend,
    MessageCollected,
    MessageProcessed,
    GetUser,

    Undefined
};

#endif // SYSTEMEVENTS_H
