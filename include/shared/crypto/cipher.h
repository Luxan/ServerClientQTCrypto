#pragma once

#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

#include "crypto.h"
#include "../package_buffer.h"
#include "key.h"

class Cipher : public CryptographyBase
{
protected:
    CryptoPP::AutoSeededRandomPool rng;
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual bool encrypt(PackageBuffer *input, PackageBuffer *output) = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual bool decrypt(PackageBuffer *input, PackageBuffer *output) = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    Cipher()
    {}

    virtual ~Cipher()
    {}
};
