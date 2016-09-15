/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <stdint.h>

/**
\class enum Status
\brief shows status of user.
*/
enum class Status : uint8_t
{
    Offline, /*! if user is not logged in */
    Away, /*! if user is logged in but inactive for a while */
    Sneezy, /*! if user is logged in but inactive for a long time */
    Busy, /*! if user is not logged in but forsedhis status to busy */
    MissYou, /*! if user is not logged in and send notice to someone to contact him asap*/
    Online /*! if user is logged in and is active */
};
