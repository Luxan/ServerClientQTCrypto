#pragma once

#include "cipher.h"

class RSACipher : public Cipher
{
private:
    CryptoPP::RSAES_OAEP_SHA_Encryptor * encryptor;
    CryptoPP::RSAES_OAEP_SHA_Decryptor * decryptor;
    Key *pubKey, *privKey;
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    RSACipher(Key * publicKey, Key * privateKey)
    {
        if (privateKey != nullptr)
        {
            CryptoPP::RSA::PrivateKey rsaPrivate;
            CryptoPP::ArraySource arraySource(privateKey->getPointerToBuffer(), privateKey->getLength(), true);
            rsaPrivate.BERDecode(arraySource);
            decryptor = new CryptoPP::RSAES_OAEP_SHA_Decryptor(rsaPrivate);
            privKey = new Key(privateKey->getPointerToBuffer(), privateKey->getLength(), Key::EncryptionKeyType::RSAPrivate);
        }
        else
        {
            decryptor = nullptr;
            privKey = nullptr;
        }
        if (publicKey != nullptr)
        {
            CryptoPP::RSA::PublicKey rsaPublic;
            CryptoPP::ArraySource arraySource(publicKey->getPointerToBuffer(), publicKey->getLength(), true);
            rsaPublic.BERDecode(arraySource);
            encryptor = new CryptoPP::RSAES_OAEP_SHA_Encryptor(rsaPublic);
            pubKey = new Key(publicKey->getPointerToBuffer(), publicKey->getLength(), Key::EncryptionKeyType::RSAPublic);
        }
        else
        {
            encryptor = nullptr;
            pubKey = nullptr;
        }
    }

    Key * getPublicKey()
    {
        return pubKey;
    }

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    RSACipher()
    {
        ///////////////////////////////////////
        // Generate Parameters
        CryptoPP::InvertibleRSAFunction params;
        params.GenerateRandomWithKeySize(rng, 4096);

        ///////////////////////////////////////
        // Create Keys
        CryptoPP::RSA::PrivateKey privateKey(params);
        CryptoPP::RSA::PublicKey publicKey(params);
        encryptor = new CryptoPP::RSAES_OAEP_SHA_Encryptor(publicKey);
        decryptor = new CryptoPP::RSAES_OAEP_SHA_Decryptor(privateKey);

        uint8_t buffer[8192];

        CryptoPP::ArraySink arraySink(&buffer[0], 8192);
        privateKey.DEREncode(arraySink);
        privKey = new Key(buffer, arraySink.TotalPutLength(), Key::EncryptionKeyType::RSAPrivate);
        CryptoPP::ArraySink arraySink2(&buffer[0], 8192);
        publicKey.DEREncode(arraySink2);
        pubKey = new Key(buffer, arraySink2.TotalPutLength(), Key::EncryptionKeyType::RSAPublic);
    }
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ~RSACipher()
    {
        if (encryptor != nullptr)
            delete encryptor;
        if (decryptor != nullptr)
            delete decryptor;
        if (privKey != nullptr)
            delete privKey;
        if (pubKey != nullptr)
            delete pubKey;
    }
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
        if (encryptor == nullptr)
            throw "Cannot encrypt package without public Key!";
        const size_t max_size = encryptor->FixedMaxPlaintextLength();
        if (0 == encryptor->FixedMaxPlaintextLength())
        {
            throw "FixedMaxPlaintextLength is 0!";
        }

        size_t offset = 0;
        while(offset < input->getLength())
        {
            int rest = input->getLength() - offset >= max_size ? max_size : input->getLength() - offset;

            // Create cipher text space
            size_t ecl = encryptor->CiphertextLength( rest );
            if ( 0 == ecl )
            {
                throw "CiphertextLength is 0!";
            }
            CryptoPP::SecByteBlock ciphertext( ecl );

            encryptor->Encrypt( rng, input->getPointerToBuffer() + offset, rest, ciphertext );

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
        if (decryptor == nullptr)
            throw "Cannot decrypt package without private Key!";

        const size_t max_size = decryptor->FixedCiphertextLength();
        if ( 0 == decryptor->FixedCiphertextLength() )
        {
            throw "CiphertextLength is 0!";
        }

        size_t offset = 0;
        while(offset < input->getLength())
        {
            int rest = input->getLength() - offset >= max_size ? max_size : input->getLength() - offset;

            // Create recovered text space
            size_t dpl = decryptor->MaxPlaintextLength( rest );
            if (0 == dpl)
            {
                throw "MaxPlaintextLength is 0!";
            }
            CryptoPP::SecByteBlock recovered( dpl );

            CryptoPP::DecodingResult result = decryptor->Decrypt(rng, input->getPointerToBuffer() + offset, rest, recovered );

            // More sanity checks
            if (!result.isValidCoding)
            {
                throw "Received invalid code to decode!";
            }

            if (result.messageLength > decryptor->MaxPlaintextLength( rest ))
            {
                throw "result.messageLength > decryptor->MaxPlaintextLength( rest )!";
            }

            recovered.resize( result.messageLength );

            output->concatBuff(recovered.BytePtr(), result.messageLength);

            offset += rest;
        }

        return true;
    }
};
