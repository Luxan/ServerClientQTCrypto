/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <stdint.h>

/**
\class
\brief
*/
class Hash
{
    const size_t hash_length;
    uint8_t buff[hash_length];
public:
    const size_t getHashLength() const
    {
        return hash_length;
    }

    uint8_t * getHashBuff() const
    {
        return buff;
    }

    Hash(uint8_t * buff, size_t hash_length):
        hash_length(hash_length), buff(buff)
    {}

    bool operator==(Hash &h)
    {
        int diff = this->getHashLength() ^ h.getHashLength();
        for(int i = 0; i < this->getHashLength() && i < h.getHashLength(); i++)
        diff |= this->getHashBuff()[i] ^ h.getHashBuff()[i];
        return diff == 0;
    }
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
};
