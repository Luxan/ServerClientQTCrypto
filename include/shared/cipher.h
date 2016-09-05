#pragma once

#include "crypto.h"
#include "iv.h"
#include "key.h"

/**
\struct
\brief
*/
struct PublicPrivateKeyPair
{
    Key * publicKey;
    Key * privateKey;
};

class Cipher : public CryptographyBase
{
protected:

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
};

//class RSACipher : public Cipher
//{
//private:
//    PublicPrivateKeyPair * keypair;

//    int padding = RSA_PKCS1_PADDING;

//    RSA * createRSA(unsigned char * key, int isPublic)
//    {
//        RSA *rsa= NULL;
//        BIO *keybio ;
//        keybio = BIO_new_mem_buf(key, -1);
//        if (keybio==NULL)
//        {
//            printf( "Failed to create key BIO");
//            return 0;
//        }
//        if(isPublic)
//        {
//            rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
//        }
//        else
//        {
//            rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
//        }
//        if(rsa == NULL)
//        {
//            printf( "Failed to create RSA");
//        }

//        return rsa;
//    }

//    int public_encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char *encrypted)
//    {
//        RSA * rsa = createRSA(key,1);
//        int result = RSA_public_encrypt(data_len, data, encrypted, rsa, padding);
//        return result;
//    }

//    int public_decrypt(unsigned char * enc_data, int data_len, unsigned char * key, unsigned char *decrypted)
//    {
//        RSA * rsa = createRSA(key,1);
//        int  result = RSA_public_decrypt(data_len, enc_data, decrypted, rsa, padding);
//        return result;
//    }

//    int private_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
//    {
//        RSA * rsa = createRSA(key,0);
//        int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
//        return result;
//    }

//    int private_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
//    {
//        RSA * rsa = createRSA(key,0);
//        int result = RSA_private_encrypt(data_len,data,encrypted,rsa,padding);
//        return result;
//    }
//public:
//    RSACipher(Key * publicKey, Key * privateKey)
//    {
//        pair->privateKey = publicKey;
//        pair->publicKey = privateKey;
//    }

//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    virtual bool encrypt(PackageBuffer *input, PackageBuffer *output)
//    {
//        unsigned char encrypted[4098]={};

//        int encrypted_length = public_encrypt(buff, size, this->getPublicKey(), encrypted);
//        if(encrypted_length == -1)
//        {
//            printLastError("Public Encrypt failed ");
//            return false;
//        }

//        delete
//    }

//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    virtual bool decrypt(PackageBuffer *input, PackageBuffer *output)
//    {

//    }

//};




class EVPCipher : public Cipher
{
protected:
    const Key * key;
    const size_t key_length;
    const IV * iv;
    const size_t iv_length;
    const EVP_CIPHER* cipherAlgorithm;
    const size_t max_data_block_length;

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    int aes_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
      unsigned char *iv, unsigned char *ciphertext)
    {
      EVP_CIPHER_CTX *ctx;

      int len;

      int ciphertext_len;

      /* Create and initialise the context */
      if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

      /* Initialise the encryption operation. IMPORTANT - ensure you use a key
       * and IV size appropriate for your cipher
       * In this example we are using 256 bit AES (i.e. a 256 bit key). The
       * IV size for *most* modes is the same as the block size. For AES this
       * is 128 bits */
      if(1 != EVP_EncryptInit_ex(ctx, cipherAlgorithm, NULL, key, iv))
        handleErrors();

      /* Provide the message to be encrypted, and obtain the encrypted output.
       * EVP_EncryptUpdate can be called multiple times if necessary
       */
      if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
      ciphertext_len = len;

      /* Finalise the encryption. Further ciphertext bytes may be written at
       * this stage.
       */
      if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
      ciphertext_len += len;

      /* Clean up */
      EVP_CIPHER_CTX_free(ctx);

      return ciphertext_len;
    }
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    int aes_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
      unsigned char *iv, unsigned char *plaintext)
    {
      EVP_CIPHER_CTX *ctx;

      int len;

      int plaintext_len;

      /* Create and initialise the context */
      if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

      /* Initialise the decryption operation. IMPORTANT - ensure you use a key
       * and IV size appropriate for your cipher
       * In this example we are using 256 bit AES (i.e. a 256 bit key). The
       * IV size for *most* modes is the same as the block size. For AES this
       * is 128 bits */
      if(1 != EVP_DecryptInit_ex(ctx, cipherAlgorithm, NULL, key, iv))
        handleErrors();

      /* Provide the message to be decrypted, and obtain the plaintext output.
       * EVP_DecryptUpdate can be called multiple times if necessary
       */
      if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
      plaintext_len = len;

      /* Finalise the decryption. Further plaintext bytes may be written at
       * this stage.
       */
      if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
      plaintext_len += len;

      /* Clean up */
      EVP_CIPHER_CTX_free(ctx);

      return plaintext_len;
    }
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    EVPCipher(Key * key, size_t key_length, IV * iv, size_t iv_length, const EVP_CIPHER* cipherAlgorithm, std::string algorithmName, size_t data_block_length):
        key(key), key_length(key_length), iv(iv), iv_length(iv_length), cipherAlgorithm(cipherAlgorithm), max_data_block_length(data_block_length)
    {
        if (key->getKeyLength() != key_length)
            throw("Algorithm " + algorithmName + " expected key with length " + std::to_string(key_length) + " got " + std::to_string(key->getKeyLength()));

        if (iv->getIVLength() != iv_length)
            throw("Algorithm " + algorithmName + " expected iv with length " + std::to_string(iv_length) + " got " + std::to_string(key->getKeyLength()));
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
        /* Buffer for ciphertext. Ensure the buffer is long enough for the
        * ciphertext which may be longer than the plaintext, dependant on the
        * algorithm and mode
        */
        unsigned char ciphertext[max_data_block_length];

        int ciphertext_len;

        size_t offset = 0;
        while(offset < input->getLength())
        {
            int rest = input->getLength() - offset >= max_data_block_length ? max_data_block_length : input->getLength() - offset;
            /* Encrypt the input data */
            ciphertext_len = aes_encrypt(input->getPointerToBuffer()[offset], rest, key, iv, ciphertext);

            output->concatBuff(ciphertext, ciphertext_len);

            offset += max_data_block_length;
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
        /* Buffer for the decrypted text */
        unsigned char decryptedtext[iv->getIvLength()];

        int decryptedtext_len;

        int offset = 0;
        while(offset < input->getLength())
        {
            int rest = input->getLength() - offset >= max_data_block_length ? max_data_block_length : input->getLength() - offset;
            /* Decrypt the input data */
            decryptedtext_len = aes_decrypt(input->getPointerToBuffer()[offset], rest, key, iv, ciphertext);

            output->concatBuff(decryptedtext, decryptedtext_len);

            offset += max_data_block_length;
        }

        return true;
    }
};

class EVP_aes_256_cbc_Cipher: public EVPCipher
{
public:
    EVP_aes_256_cbc_Cipher(Key * key, IV * iv):
        EVPCipher(key, 256, iv, 128, EVP_aes_256_cbc(), "EVP_aes_256_cbc", 128)
    {}
};

class EVP_aes_256_xts_Cipher: public EVPCipher
{
public:
    EVP_aes_256_xts_Cipher(Key * key, IV * iv):
        EVPCipher(key, 512, iv, 128, EVP_aes_256_xts(), "EVP_aes_256_xts", 128)
    {}
};

/*
class EVP_aes_256_ccm_Cipher: public EVPCipher
{
public:
    EVP_aes_256_ccm_Cipher(Key * key, IV * iv):
        EVPCipher(key, iv, EVP_aes_256_ccm())
    {
        if (key->getKeyLength() != )
            throw("Algorithm EVP_aes_256_ccm expected key with length 512 got " + std::to_string(key->getKeyLength()));
    }
};
*/
