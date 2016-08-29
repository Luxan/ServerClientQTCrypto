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
    RequestStartMessageCollector,
    RequestSleepMessageCollector,
    ResponseStartMessageCollector,
    ResponseSleepMessageCollector,

    RequestStartMessageProcessor,
    RequestSleepMessageProcessor,
    ResponseStartMessageProcessor,
    ResponseSleepMessageProcessor,

    RequestStartMessageWorker,
    RequestSleepMessageWorker,
    ResponseStartMessageWorker,
    ResponseSleepMessageWorker,

    RequestStartTcpChannel,
    RequestSleepTcpChannel,
    ResponseStartTcpChannel,
    ResponseSleepTcpChannel,

    ErrorMessageCollector,
    ErrorTcpChannel,
    ErrorMessageProcessor,
    ErrorGui,
    ErrorMessageWorker,

    PackageReceived,
    PackageToSend,
    MessageCollected,
    MessageProcessed,
    GetUser,

    Undefined
};

#endif // SYSTEMEVENTS_H
