/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <stdint.h>
#include "../buffer.h"
/**
\class
\brief
*/
class Hash
{
    uint8_t * buff;
    const BUFF_SIZE hash_length;
public:
    BUFF_SIZE getLength() const
    {
        return hash_length;
    }

    Hash(uint8_t * buff, size_t hash_length):
        buff(buff), hash_length(hash_length)
    {}

    virtual ~Hash()
    {
        delete[] buff;
    }

    bool operator==(Hash &h)const
    {
        int diff = this->getLength() ^ h.getLength();
        for(uint8_t i = 0; i < this->getLength() && i < h.getLength(); i++)
        diff |= this->getBuff()[i] ^ h.getBuff()[i];
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
