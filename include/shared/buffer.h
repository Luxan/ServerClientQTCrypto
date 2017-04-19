#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <string>
//!declare how much data can be stored in buffer. uint16_t = max 65536 bytes
#define BUFF_SIZE uint16_t

/*!
\class Buffer 
\memory clears all data that stored inside
\brief class used to store byte data. Can store max 65536 bytes(depends on size of BUFF_SIZE)
*/
class Buffer
{
protected:
    uint8_t *buff; 	/*!pointer to data buffer*/
    BUFF_SIZE length;	/*!length of data buffer*/

public:
    /*!
    \brief Constructor to COPY and store given data.
    \param _buff - pointer to data that will be stored
    \param _length - length of data that will be stored
    */
    Buffer(uint8_t *_buff, BUFF_SIZE _length)
    {
        buff = new uint8_t[_length + 1];
        length = _length;
        memcpy(buff, _buff, _length);
    }

    /*!
    \brief Constructor to create empty buffer with given size.
    \param _length - length of empty buffer that will be created
    */
    Buffer(BUFF_SIZE _length)
    {
        buff = new uint8_t[_length + 1];
        length = _length;
    }

    /*!
    \brief Constructor to create empty buffer.
    */
    Buffer()
    {
        buff = nullptr;
        length = 0;
    }

    /*!
    \brief resize empty buffer.
    \param _length - length of the buffer that will be created.
    \pre buffer must be created using default contructor!
    \throw Cannot resize not empty buffer! if buffer is not empty
    */
    void resize(BUFF_SIZE _length)
    {
        if (buff != nullptr || length != 0)
            throw "Cannot resize not empty buffer!";
        length = _length;
        buff = new uint8_t[_length + 1];
    }

    /*!
    \return uint8_t * - pointer to data buffer
    \brief returns pointer to data buffer stored inside class object
    */
    virtual uint8_t * getPointerToBuffer() const
    {
        return buff;
    }

    /*!
    \brief returns true if contents of two Buffers are the same
    \return bool - true if contents are the same
    */
    bool operator== (Buffer &buffer)
    {
        if (buffer.getPointerToBuffer() == nullptr)
        {
            if (this->getPointerToBuffer() == nullptr)
                return true;
            else
                return false;
        }
        if (buffer.getLength() != this->getLength())
            return false;

        for (int i = 0; i < this->getLength(); i++)
        {
            if (*(this->getPointerToBuffer() + i) != *(buffer.getPointerToBuffer() + i))
                return false;
        }
        return true;
    }

    /*!
    \brief returns true if contents of two Buffers are not the same
    \return bool - true if contents are not the same
    */
    bool operator!= (Buffer &buffer)
    {
        return !this->operator==(buffer);
    }

    /*!
    \return size of data
    \brief returns size of data buffer stored inside class object
    */
    virtual BUFF_SIZE getLength() const
    {
        return length;
    }

    /*!
    \brief destructor that destroy all data stored inside class object
    */
    virtual ~Buffer()
    {
        delete[] buff;
    }
};
