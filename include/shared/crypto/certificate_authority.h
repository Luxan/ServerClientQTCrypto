#pragma once

#include "../buffer.h"

class CertificateAuthority
{
    Buffer * ca;
public:
    CertificateAuthority(Buffer * ca):
        ca(ca)
    {}

    ~CertificateAuthority()
    {
        if (ca != nullptr)
            delete ca;
    }

    Buffer * getBuffer()
    {
        return ca;
    }
};
