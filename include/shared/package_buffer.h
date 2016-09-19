#pragma once

#include "buffer.h"

/*!
\class PackageBuffer
\brief store bytes of package used in network communication.
*/
class PackageBuffer : public Buffer
{
private:
	BUFF_SIZE offset; /*! offset is increased if fillBuffer was called to make filled bytes inaccessible */

public:
	/*!
	\brief give length of buffer with offset stored inside of object of class
	\return length of buffer with offset.
	*/
    virtual BUFF_SIZE getLength() const;

	/*!
	\brief give pointer to buffer with offset stored inside of object of class
	\return pointer to buffer with offset(if fillBuffer was called then offset will be increased so given data for filling will be unaccessible).
	*/
	virtual uint8_t * getPointerToBuffer() const;

	/*!
	\brief COPY data stored in object class to given buffer and increase offset of data that is stored inside object class
	\param _buff - pointer to buffer to fill
	\param size - length of buffer to fill
	\throw "Not enough memory in PackageBuffer" if stored data length is smaller than given to fill
	\pre length of initial data length must be equal or more than length of given to fill
	\example if PackageBuffer stored 0123456789 and fillBuffer called to fill buffer with length 5 it fills 01234. 
	If next time fillBuffer will be called to fill buffer with length 5 it will fill 56789.
	*/
	void fillBuffer(uint8_t *_buff, BUFF_SIZE size);

	/*!
	\brief COPY and store data inside object class
	\param _buff - pointer to buffer to store
	\param _length - length of buffer to store
	*/
	PackageBuffer(const uint8_t *_buff, BUFF_SIZE _length);

	/*!
	\brief Destructor that destroy all data stored inside object class
	*/
	virtual ~PackageBuffer();

	/*!
	\param _buff - pointer to buffer to concantenate with
	\param _length - length of buffer to concantenate with
	\brief create new buffer and copy initial data stored in object class and received buffer as parameter.
	*/
	void concatBuff(uint8_t *_buff, BUFF_SIZE _length);

	/*!
	\return pointer to new copy of actual object.
	\brief create new object of a class and copy all data to it then erase all data in self object. Return created copy of an object. 
	*/
	PackageBuffer *copyToNewAndClear();
};
