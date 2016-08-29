/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include "../shared/key.h"
#include "../shared/buffer.h"

/**
\struct
\brief
*/
struct PublicPrivateKeyPair
{
    Key * publicKey;
    Key * privateKey;
};

class Cipher
{
protected:
    PublicPrivateKeyPair pair;
public:
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    Cipher()
    {
        pair.privateKey = new Key();
        pair.publicKey = new Key();
    }
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~Cipher()
    {
        delete pair.privateKey;
        delete pair.publicKey;
    }

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual void encrypt(uint8_t ** buff, size_t & size)
    {

    }

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual void decrypt(uint8_t ** buff, size_t & size)
    {

    }

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual const Key * getPublicKey() const
    {
        return pair.publicKey;
    }
};

class CipherPassword : public Cipher
{
public:
    static int64_t generateSalt()
    {
        return rand();
    }
};

class HashFunction
{
public:
    virtual uint8_t * hash(int64_t &salt, const uint8_t * password)=0;
    virtual size_t hashSize()=0;
};

class Hash : public Buffer
{
private:
    Hash(int64_t &salt, const uint8_t * password, HashFunction *func):
        Buffer(func->hash(salt, password), func->hashSize())
    {}
};
