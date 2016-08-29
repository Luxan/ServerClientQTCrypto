/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <stdint.h>

/**
\class
\brief
*/
enum class Status : uint8_t
{
    Offline,
    Away,
    Sneezy,
    Busy,
    MissYou,
    Online
};
