#include "../../include/shared/buffer_spitter.h"


bufferSpitter::bufferSpitter(uint8_t *_buff, uint8_t _length):
    Buffer(_buff, _length)
{}

void bufferSpitter::splitBufferIntoList(std::list<PackageBuffer *> &list, PackageBuffer *incompletePackageBuffer, size_t &incompletePackageFullLength)
{
    //check if temporary package exist
    bool isTempPackageExist = incompletePackageFullLength > 0;

    //check if somth is still in th input buffer
    while (length > 0)
    {
        //check if temporary buffer is exist then try to add bytes to it
        if (isTempPackageExist)
        {
            size_t needToFull = incompletePackageFullLength - incompletePackageBuffer->getLength();
            //if in input buffer has more bytes than temporary buffer need to be full
            if (length >= needToFull)
            {
                //add needed amount bytes from input buffer to temporary buffer
                incompletePackageBuffer->concatBuff(buff, needToFull);
                buff += needToFull;
                //push back ready to process new buffer
                list.push_back(incompletePackageBuffer->copyToNewAndClear());
                //substruct bytes that needed to full temporary buffer
                length -= needToFull;
                //set temporary buffer package size to 0 that means we already received buffer and w8ting on new one
                incompletePackageFullLength = 0;
                isTempPackageExist = false;
            }
            //if in input buffer has less bytes than temporary buffer need to be full
            else
            {
                //add all bytes from input buffer to temporary buffer
                incompletePackageBuffer->concatBuff(buff, length);
                buff += length;
                //we got all bytes from buffer
                length = 0;
            }
        }
        //if temporary buffer is not exist then create new temporary buffer using bytes from input buffer
        else
        {
            //extract full package length
            uint8_t packLength = *(buff++);
            length--;
            //if full package length equals zero then skip that package
            if (packLength == 0)
                continue;
            //if input buffer has enough bytes to fill in full package then try to fill
            if (packLength <= length)
            {
                //push back ready to process new buffer
                list.push_back(new PackageBuffer(buff, packLength));
                buff +=  packLength;
                //substruct bytes that needed to full temporary buffer
                length -= packLength;
            }
            //if input buffer has not enough bytes to fill in full package then try to fill in temporary package and w8 on next portion of bytes from input
            else
            {
                isTempPackageExist = true;
                incompletePackageFullLength = packLength;

                incompletePackageBuffer->concatBuff(buff, length);
                buff += length;
                //got all bytet to fill in temporary package so we dont have bytes left
                length = 0;
            }
        }
    }
}
