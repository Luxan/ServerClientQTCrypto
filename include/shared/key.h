/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#define KEY_LENGTH 64

#include <stdint.h>

/**
\class
\brief
*/
class Key
{
    uint8_t buff[KEY_LENGTH];
public:
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    const uint8_t * getBuff()const
    {
        return buff;
    }

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    Key();
};

