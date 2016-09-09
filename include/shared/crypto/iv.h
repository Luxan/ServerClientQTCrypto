#pragma once

#include "crypto.h"

class IV
{
    const size_t iv_length;
    uint8_t * buff;
public:
    size_t getIVLength() const
    {
        return iv_length;
    }

    IV(size_t iv_length):
        iv_length(iv_length)
    {
        buff = CryptographyBase::createRandomBuffer(iv_length);
    }

    ~IV()
    {
        delete buff;
    }

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    const uint8_t * getBuff()const
    {
        return buff;
    }
};
class IV64 : IV
{
public:
    IV64():
        IV(64)
    {}
};
class IV128 : IV
{
public:
    IV128():
        IV(128)
    {}
};
class IV256 : IV
{
public:
    IV256():
        IV(256)
    {}
};
class IV512 : IV
{
public:
    IV512():
        IV(512)
    {}
};
class IV1024 : IV
{
public:
    IV1024():
        IV(1024)
    {}
};
