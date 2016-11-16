#pragma once

#include <cryptopp/sha3.h>
#include "crypto.h"
#include "../package_buffer.h"
#include "hash.h"

class Hasher : public CryptographyBase
{
protected:
//    uint8_t * digest_message(const uint8_t *message, size_t message_len, int itherationsCount) = 0;
public:
    virtual Hash * createHash(PackageBuffer * input, int itherationsCount) = 0;
};


class SHA3 : public Hasher
{
private:
    CryptoPP::SHA3 * hashingAlgorithm;
    const int digestsize;

    uint8_t * digest_message(Buffer * message, uint32_t itherationsCount)
    {
        if (message == nullptr)
            throw ("SHA3: message is nullptr!");

        uint8_t * digest = new uint8_t[digestsize];

        hashingAlgorithm->CalculateDigest(digest, message->getPointerToBuffer(), message->getLength());

        for (uint32_t i = 0; i < itherationsCount; i++)
            hashingAlgorithm->CalculateDigest(digest, digest, digestsize);

        return digest;
    }
protected:
    SHA3(CryptoPP::SHA3 * hashingAlgorithm, const int DIGESTSIZE):
        hashingAlgorithm(hashingAlgorithm), digestsize(DIGESTSIZE)
    {}
    virtual  ~SHA3()
    {
        delete hashingAlgorithm;
    }
public:
    int getDigestSize()const
    {
        return digestsize;
    }

    Hash * createHash(PackageBuffer * input, int itherationsCount)
    {
        return new Hash(digest_message(input, itherationsCount), getDigestSize());
    }
    Hash * createHash(Buffer * input, int itherationsCount)
    {
        return new Hash(digest_message(input, itherationsCount), getDigestSize());
    }
};

class Sha256Hasher : public SHA3
{
    Sha256Hasher():
        SHA3(new CryptoPP::SHA3_256(), CryptoPP::SHA3_256::DIGESTSIZE)
    {}
public:
    static Sha256Hasher & getInstance()
    {
        static Sha256Hasher sh;
        return sh;
    }
};
