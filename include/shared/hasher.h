#pragma once

#include "crypto.h"

class Hasher : public CryptographyBase
{
private:
    const EVP_MD * hashingAlgorithm;
protected:
    void digest_message(const uint8_t *message, size_t message_len, uint8_t **digest, uint32_t *digest_len)
    {
        EVP_MD_CTX *mdctx;

        if((mdctx = EVP_MD_CTX_create()) == NULL)
            handleErrors();

        if(1 != EVP_DigestInit_ex(mdctx, hashingAlgorithm, NULL))
            handleErrors();

        if(1 != EVP_DigestUpdate(mdctx, message, message_len))
            handleErrors();

        if((*digest = (uint8_t *)OPENSSL_malloc(EVP_MD_size(hashingAlgorithm))) == NULL)
            handleErrors();

        if(1 != EVP_DigestFinal_ex(mdctx, *digest, digest_len))
            handleErrors();

        EVP_MD_CTX_destroy(mdctx);
    }
public:
    virtual Hash * createHash(PackageBuffer * input) = 0;

    Hasher(const EVP_MD * hashingAlgorithm):
        hashingAlgorithm(hashingAlgorithm)
    {}
};

class Sha256Hasher : public Hasher
{
public:
    Sha256Hasher():
        Hasher(EVP_sha256())
    {}

    Hash * createHash(PackageBuffer * input)
    {
        uint32_t digest_len = 0;
        uint8_t * buff;
        digest_message(input->getPointerToBuffer(), input->getLength(), &buff, &digest_len);
        return new Hash(buff, digest_len);
    }
};
