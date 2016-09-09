#pragma once

#include <cryptopp/sha3.h>
#include "crypto.h"
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
    const int DIGESTSIZE;

    uint8_t * digest_message(const uint8_t *message, size_t message_len, int itherationsCount)
    {
        if (message == nullptr)
            throw ("SHA3: message is nullptr!");

        if (itherationsCount < 1)
            throw ("SHA3: itherationsCount is fewer than 1!");

        uint8_t * digest = new uint8_t[DIGESTSIZE];

        hashingAlgorithm->CalculateDigest(digest, message, message_len);

        for (int i = 1; i < itherationsCount; i++)
            hashingAlgorithm->CalculateDigest(digest, digest, DIGESTSIZE);

        return digest;
    }
public:
    int getDigestSize()const
    {
        return DIGESTSIZE;
    }

    SHA3(CryptoPP::SHA3 * hashingAlgorithm, const int DIGESTSIZE):
        hashingAlgorithm(hashingAlgorithm), DIGESTSIZE(DIGESTSIZE)
    {}

   virtual  ~SHA3()
    {
        delete hashingAlgorithm;
    }

    Hash * createHash(PackageBuffer * input, int itherationsCount)
    {
        return new Hash(digest_message(input->getPointerToBuffer(), input->getLength(), itherationsCount), getDigestSize());
    }
};

class Sha256Hasher : public SHA3
{
public:
    Sha256Hasher():
        SHA3(new CryptoPP::SHA3_256(), CryptoPP::SHA3_256::DIGESTSIZE)
    {}
};
