#include "../../include/shared/buffer_spitter.h"


bufferSpitter::bufferSpitter(uint8_t *_buff, uint16_t _length):
    Buffer(_buff, _length)
{}

void bufferSpitter::splitBufferIntoList(std::list<PackageBuffer *> &list, PackageBuffer *incompletePackageBuffer, size_t &incompletePackageFullLength)
{
    //check if temporary package exist
    bool isTempPackageExist = incompletePackageFullLength > 0;

    //check if somth is still in th input buffer
    while (length > 0)
    {
        //check if temporary buffer is exist then try to add uint8_ts to it
        if (isTempPackageExist)
        {
            size_t needToFull = incompletePackageFullLength - incompletePackageBuffer->getLength();
            //if in input buffer has more uint8_ts than temporary buffer need to be full
            if (length >= needToFull)
            {
                //add needed amount uint8_ts from input buffer to temporary buffer
                incompletePackageBuffer->concatBuff(buff, needToFull);
                buff += needToFull;
                //push back ready to process new buffer
                list.push_back(incompletePackageBuffer->copyToNewAndClear());
                //substruct uint8_ts that needed to full temporary buffer
                length -= needToFull;
                //set temporary buffer package size to 0 that means we already received buffer and w8ting on new one
                incompletePackageFullLength = 0;
                isTempPackageExist = false;
            }
            //if in input buffer has less uint8_ts than temporary buffer need to be full
            else
            {
                //add all uint8_ts from input buffer to temporary buffer
                incompletePackageBuffer->concatBuff(buff, length);
                buff += length;
                //we got all uint8_ts from buffer
                length = 0;
            }
        }
        //if temporary buffer is not exist then create new temporary buffer using uint8_ts from input buffer
        else
        {
            //extract full package length
            uint16_t packLength = *((uint16_t*)buff);
            ((uint16_t*)buff)++;
            length -= sizeof(uint16_t);
            //if full package length equals zero then skip that package
            if (packLength == 0)
                continue;
            //if input buffer has enough bytes to fill in full package then try to fill
            if (packLength <= length)
            {
                //push back ready to process new buffer
                list.push_back(new PackageBuffer(buff, packLength));
                buff +=  packLength;
                //substruct uint8_ts that needed to full temporary buffer
                length -= packLength;
            }
            //if input buffer has not enough uint8_ts to fill in full package then try to fill in temporary package and w8 on next portion of uint8_ts from input
            else
            {
                isTempPackageExist = true;
                incompletePackageFullLength = packLength;

                incompletePackageBuffer->concatBuff(buff, length);
                buff += length;
                //got all uint8_tt to fill in temporary package so we dont have uint8_ts left
                length = 0;
            }
        }
    }
}
