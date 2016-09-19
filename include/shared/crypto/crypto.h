#pragma once

#include <stdio.h>

#include "../log.h"

class CryptographyBase
{
public:
    static uint8_t * createRandomBuffer(size_t size)
    {
        return new uint8_t[size];
    }
};
