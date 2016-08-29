#pragma once

#include <stdint.h>
#include <list>
#include "buffer.h"
#include "package_buffer.h"

class bufferSpitter : public Buffer
{

public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bufferSpitter(uint8_t *_buff, uint8_t _length);

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void splitBufferIntoList(std::list<PackageBuffer *> &list, PackageBuffer *incompletePackageBuffer, size_t &incompletePackageFullLength);
};
