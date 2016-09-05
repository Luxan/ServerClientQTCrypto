#pragma once

#include <openssl/conf.h>

#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>

#include "../server/slog.h"

class CryptographyBase
{
public:
    static void InitializeOpenSSL()
    {
        /* Load the human readable error strings for libcrypto */
        ERR_load_crypto_strings();

        /* Load all digest and cipher algorithms */
        OpenSSL_add_all_algorithms();

        /* Load config file, and other important initialisation */
        OPENSSL_config(NULL);
    }

    static void CleanOpenSSL()
    {
        /* Removes all digests and ciphers */
        EVP_cleanup();

        /* if you omit the next, a small leak may be left when you make use of the BIO (low level API) for e.g. base64 transformations */
        CRYPTO_cleanup_all_ex_data();

        /* Remove error strings */
        ERR_free_strings();
    }

    void handleErrors()
    {
        char * err = new char[130];
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        SLog::logError() << " Crypto: " + std::string(err);
        delete[] err;
    }

    static uint8_t * createRandomBuffer(size_t size)
    {
        return new uint8_t[size];
    }
};
