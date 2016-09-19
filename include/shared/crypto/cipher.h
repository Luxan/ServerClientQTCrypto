#pragma once

#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

#include "crypto.h"
#include "../package_buffer.h"

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

class RSACipher : public Cipher
{
private:
    CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor;
    CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor;
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    RSACipher(CryptoPP::RSA::PublicKey &publicKey, CryptoPP::RSA::PrivateKey &privateKey):
        encryptor(publicKey), decryptor(privateKey)
    {}
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual bool encrypt(PackageBuffer *input, PackageBuffer *output)
    {
        const size_t max_size = encryptor.FixedMaxPlaintextLength();
        assert( 0 != encryptor.FixedMaxPlaintextLength() );

        size_t offset = 0;
        while(offset < input->getLength())
        {
            int rest = input->getLength() - offset >= max_size ? max_size : input->getLength() - offset;

            // Create cipher text space
            size_t ecl = encryptor.CiphertextLength( rest );
            assert( 0 != ecl );
            CryptoPP::SecByteBlock ciphertext( ecl );

            encryptor.Encrypt( rng, input->getPointerToBuffer() + offset, rest, ciphertext );

            output->concatBuff(ciphertext.BytePtr(), ecl);

            offset += rest;
        }

        return true;
    }

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual bool decrypt(PackageBuffer *input, PackageBuffer *output)
    {
        const size_t max_size = decryptor.FixedCiphertextLength();
        assert( 0 != decryptor.FixedCiphertextLength() );

        size_t offset = 0;
        while(offset < input->getLength())
        {
            int rest = input->getLength() - offset >= max_size ? max_size : input->getLength() - offset;

            // Create recovered text space
            size_t dpl = decryptor.MaxPlaintextLength( rest );
            assert( 0 != dpl );
            CryptoPP::SecByteBlock recovered( dpl );

            CryptoPP::DecodingResult result = decryptor.Decrypt(rng, input->getPointerToBuffer() + offset, rest, recovered );

            // More sanity checks
            assert( result.isValidCoding );
            assert( result.messageLength <= decryptor.MaxPlaintextLength( rest ) );

            recovered.resize( result.messageLength );

            output->concatBuff(recovered.BytePtr(), result.messageLength);

            offset += rest;
        }

        return true;
    }
};
