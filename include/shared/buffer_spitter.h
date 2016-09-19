#pragma once

#include <stdint.h>
#include <list>
#include "buffer.h"
#include "package_buffer.h"
/*!
\class Buffer Splitter
\brief used to split one huge block of received data into small packages. First 2 bytes(depends on BUFF_SIZE declaration) are size of buffer tht will be splitted
\example buffer |0x00 0x0a|0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09| will be splitted into |0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09| buffer.
\memory as class Buffer it destroys all initial data stored inside class object.
*/
class bufferSpitter : public Buffer
{
public:
    /*!
    \brief constructor that store initial data inside object class
    \param _buff - pointer to initial huge data buffer 
    \param _length - length of initial huge data buffer 
    */
    bufferSpitter(uint8_t *_buff, BUFF_SIZE _length);

    /*!
    \param &list - reference to list of PackageBuffer where will be stored splitted packages
    \param incompletePackageBuffer - pointer to PackageBuffer where are stored bytes of incomplete package. Must not be null.  
    \param &incompletePackageFullLength - size of incomplete package full length. Stored bytes inside incompletePackageBuffer + remain bytes.
    \brief splits initial huge buffer data into smaller package buffers.
    \pre initial huge data buffer must be present!
    */
    void splitBufferIntoList(std::list<PackageBuffer *> &list, PackageBuffer *incompletePackageBuffer, BUFF_SIZE &incompletePackageFullLength);
};
