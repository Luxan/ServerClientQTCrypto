/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef PUBLICPRIVATEKEYPAIR_H
#define PUBLICPRIVATEKEYPAIR_H

#include "../shared/key.h"

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
private:
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
    ~Cipher()
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
    void encrypt(uint8_t ** buff, size_t & size)
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
    void decrypt(uint8_t ** buff, size_t & size)
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
    const Key * getPublicKey() const
    {
        return pair.publicKey;
    }
};

#endif // PUBLICPRIVATEKEYPAIR_H
