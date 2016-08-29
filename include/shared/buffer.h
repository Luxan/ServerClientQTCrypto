#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <string>


class Buffer
{
protected:
    uint8_t *buff;
    uint8_t length;

public:
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    Buffer(uint8_t *_buff, uint8_t _length):
        buff(_buff),length(_length)
    {}

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual uint8_t * getPointerToBuffer() const
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
    virtual size_t getLength() const
    {
        return length;
    }

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~Buffer()
    {
		if (buff != nullptr)
			delete[] buff;
	}
};

