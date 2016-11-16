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
class Hash : public Buffer
{
public:
    Hash(uint8_t * buff, BUFF_SIZE hash_length):
        Buffer(buff, hash_length)
    {}

    Hash(){}

    bool operator==(Hash &h)const
    {
        int diff = this->getLength() ^ h.getLength();
        for(uint8_t i = 0; i < this->getLength() && i < h.getLength(); i++)
        diff |= this->getPointerToBuffer()[i] ^ h.getPointerToBuffer()[i];
        return diff == 0;
    }
};
