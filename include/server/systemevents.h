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
    //Database
    RequestStartDatabase,
    RequestSleepDatabase,
    ResponseStartDatabase,
    ResponseSleepDatabase,
    //Encryption Processpor
    RequestStartEncryptionProcessor,
    RequestSleepEncryptionProcessor,
    ResponseStartEncryptionProcessor,
    ResponseSleepEncryptionProcessor,
    //Decryption Processor
    RequestStartDecryptionProcessor,
    RequestSleepDecryptionProcessor,
    ResponseStartDecryptionProcessor,
    ResponseSleepDecryptionProcessor,
    //message Collector
    RequestStartMessageCollector,
    RequestSleepMessageCollector,
    ResponseStartMessageCollector,
    ResponseSleepMessageCollector,
    //message Processor
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

    RequestStartDecryptionWorker,
    RequestSleepDecryptionWorker,
    ResponseStartDecryptionWorker,
    ResponseSleepDecryptionWorker,

//errors
    ErrorDatabase,
    ErrorCannotRemoveUserFromDatabase,
    ErrorCannotFindUserFromDatabase,
    ErrorEncryptionProcessor,
    ErrorDecryptionProcessor,
    ErrorMessageCollector,
    ErrorTcpChannel,
    ErrorMessageProcessor,
    ErrorMessageWorker,
    ErrorDecryptionWorker,
    ErrorEncryptionWorker,
    ErrorExecutionTask,
    ErrorReceivedBadPackage,
    ErrorThread,

//data events
    EncryptPackage,
    EncryptPackageWithRSA,
    DecryptPackage,
    PackageReceived,
    PackageKeyAgreementReceived,
    PackageToSend,
    MessageCollected,
    AttemptToLogIn,
    MessageKeyAgreementCollected,
    MessageProcessed,
    MessageKeyAgreementProcessed,
    DatabaseGetUserByID,
    DatabaseLoginUser,
    DatabaseDetermineUserSalt,
    DatabaseLoginSuccessfull,
    DatabaseUserSalt,
    DatabaseRegisterUser,
    DatabaseDeleteUser,
    DatabaseMissingMessages,
    DatabaseGetMissingMessages,
//Undefuned Event
    Undefined
};

#endif // SYSTEMEVENTS_H
