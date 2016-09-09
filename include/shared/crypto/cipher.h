#pragma once

//#include <cryptopp/.h>

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
    //virtual bool encrypt(PackageBuffer *input, PackageBuffer *output) = 0;

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
   // virtual bool decrypt(PackageBuffer *input, PackageBuffer *output) = 0;
};


//AutoSeededRandomPool rnd;

//// Generate a random key
//SecByteBlock key(0x00, AES::DEFAULT_KEYLENGTH);
//rnd.GenerateBlock( key, key.size() );

//// Generate a random IV
//byte iv[AES::BLOCKSIZE];
//rnd.GenerateBlock(iv, AES::BLOCKSIZE);

//char plainText[] = "Hello! How are you.";
//int messageLen = (int)strlen(plainText) + 1;

////////////////////////////////////////////////////////////////////////////
//// Encrypt

//CFB_Mode<AES>::Encryption cfbEncryption(key, key.size(), iv);
//cfbEncryption.ProcessData((byte*)plainText, (byte*)plainText, messageLen);

////////////////////////////////////////////////////////////////////////////
//// Decrypt

//CFB_Mode<AES>::Decryption cfbDecryption(key, key.size(), iv);
//cfbDecryption.ProcessData((byte*)plainText, (byte*)plainText, messageLen);

//class EVPCipher : public Cipher
//{
//protected:
//    const Key * key;
//    const size_t key_length;
//    const IV * iv;
//    const size_t iv_length;
//    const EVP_CIPHER* cipherAlgorithm;
//    const size_t max_data_block_length;

//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    int aes_encrypt(uint8_t *plaintext, int plaintext_len, uint8_t *key,
//      uint8_t *iv, uint8_t *ciphertext)
//    {
//      EVP_CIPHER_CTX *ctx;

//      int len;

//      int ciphertext_len;

//      /* Create and initialise the context */
//      if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

//      /* Initialise the encryption operation. IMPORTANT - ensure you use a key
//       * and IV size appropriate for your cipher
//       * In this example we are using 256 bit AES (i.e. a 256 bit key). The
//       * IV size for *most* modes is the same as the block size. For AES this
//       * is 128 bits */
//      if(1 != EVP_EncryptInit_ex(ctx, cipherAlgorithm, NULL, key, iv))
//        handleErrors();

//      /* Provide the message to be encrypted, and obtain the encrypted output.
//       * EVP_EncryptUpdate can be called multiple times if necessary
//       */
//      if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
//        handleErrors();
//      ciphertext_len = len;

//      /* Finalise the encryption. Further ciphertext uint8_ts may be written at
//       * this stage.
//       */
//      if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
//      ciphertext_len += len;

//      /* Clean up */
//      EVP_CIPHER_CTX_free(ctx);

//      return ciphertext_len;
//    }
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    int aes_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
//      unsigned char *iv, unsigned char *plaintext)
//    {
//      EVP_CIPHER_CTX *ctx;

//      int len;

//      int plaintext_len;

//      /* Create and initialise the context */
//      if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

//      /* Initialise the decryption operation. IMPORTANT - ensure you use a key
//       * and IV size appropriate for your cipher
//       * In this example we are using 256 bit AES (i.e. a 256 bit key). The
//       * IV size for *most* modes is the same as the block size. For AES this
//       * is 128 bits */
//      if(1 != EVP_DecryptInit_ex(ctx, cipherAlgorithm, NULL, key, iv))
//        handleErrors();

//      /* Provide the message to be decrypted, and obtain the plaintext output.
//       * EVP_DecryptUpdate can be called multiple times if necessary
//       */
//      if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
//        handleErrors();
//      plaintext_len = len;

//      /* Finalise the decryption. Further plaintext uint8_ts may be written at
//       * this stage.
//       */
//      if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
//      plaintext_len += len;

//      /* Clean up */
//      EVP_CIPHER_CTX_free(ctx);

//      return plaintext_len;
//    }
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    EVPCipher(Key * key, size_t key_length, IV * iv, size_t iv_length, const EVP_CIPHER* cipherAlgorithm, std::string algorithmName, size_t data_block_length):
//        key(key), key_length(key_length), iv(iv), iv_length(iv_length), cipherAlgorithm(cipherAlgorithm), max_data_block_length(data_block_length)
//    {
//        if (key->getKeyLength() != key_length)
//            throw("Algorithm " + algorithmName + " expected key with length " + std::to_string(key_length) + " got " + std::to_string(key->getKeyLength()));

//        if (iv->getIVLength() != iv_length)
//            throw("Algorithm " + algorithmName + " expected iv with length " + std::to_string(iv_length) + " got " + std::to_string(key->getKeyLength()));
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
//        /* Buffer for ciphertext. Ensure the buffer is long enough for the
//        * ciphertext which may be longer than the plaintext, dependant on the
//        * algorithm and mode
//        */
//        uint8_t ciphertext[max_data_block_length];

//        int ciphertext_len;

//        size_t offset = 0;
//        while(offset < input->getLength())
//        {
//            int rest = input->getLength() - offset >= max_data_block_length ? max_data_block_length : input->getLength() - offset;
//            /* Encrypt the input data */
//            ciphertext_len = aes_encrypt(
//                        input->getPointerToBuffer() + offset,
//                        rest,
//                        key,
//                        iv,
//                        ciphertext);

//            output->concatBuff(ciphertext, ciphertext_len);

//            offset += max_data_block_length;
//        }

//        return true;
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
//        /* Buffer for the decrypted text */
//        unsigned char decryptedtext[iv->getIvLength()];

//        int decryptedtext_len;

//        int offset = 0;
//        while(offset < input->getLength())
//        {
//            int rest = input->getLength() - offset >= max_data_block_length ? max_data_block_length : input->getLength() - offset;
//            /* Decrypt the input data */
//            decryptedtext_len = aes_decrypt(input->getPointerToBuffer()[offset], rest, key, iv, ciphertext);

//            output->concatBuff(decryptedtext, decryptedtext_len);

//            offset += max_data_block_length;
//        }

//        return true;
//    }
//};

//class EVP_aes_256_cbc_Cipher: public EVPCipher
//{
//public:
//    EVP_aes_256_cbc_Cipher(Key * key, IV * iv):
//        EVPCipher(key, 256, iv, 128, EVP_aes_256_cbc(), "EVP_aes_256_cbc", 128)
//    {}
//};

//class EVP_aes_256_xts_Cipher: public EVPCipher
//{
//public:
//    EVP_aes_256_xts_Cipher(Key * key, IV * iv):
//        EVPCipher(key, 512, iv, 128, EVP_aes_256_xts(), "EVP_aes_256_xts", 128)
//    {}
//};

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
