#pragma once

#include "../buffer.h"
#include "key.h"


class Certificate
{
    Buffer * certificate;
public:
    Certificate(uint8_t *_buff, BUFF_SIZE _length)
    {
        certificate = new Buffer(_buff, _length);
    }

    Certificate(Buffer *_buff):
        certificate(_buff)
    {}

    Buffer * getBuffer()
    {
        return certificate;
    }

    ~Certificate()
    {
        delete certificate;
    }
};
