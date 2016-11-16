#pragma once

#include "crypto.h"
#include "../buffer.h"

class IV : public Buffer
{
public:
    IV(size_t iv_length):
        Buffer(CryptographyBase::createRandomBuffer(iv_length), iv_length)
    {}
};
class IV64 : public IV
{
public:
    IV64():
        IV(64)
    {}
};
class IV128 : public IV
{
public:
    IV128():
        IV(128)
    {}
};
class IV256 : public IV
{
public:
    IV256():
        IV(256)
    {}
};
class IV512 : public IV
{
public:
    IV512():
        IV(512)
    {}
};
class IV1024 : public IV
{
public:
    IV1024():
        IV(1024)
    {}
};
