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
    \return uint8_t * - pointer to data buffer
    \brief returns pointer to data buffer stored inside class object
    */
    virtual uint8_t * getPointerToBuffer() const
    {
        return buff;
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
        if (buff != nullptr)
            delete[] buff;
    }
};
