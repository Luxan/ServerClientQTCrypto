#include "../../include/shared/package_buffer.h"

BUFF_SIZE PackageBuffer::getLength() const
{
    return length - offset;
}

uint8_t * PackageBuffer::getPointerToBuffer() const
{
	return buff + offset;
}

void PackageBuffer::fillBuffer(uint8_t *_buff, BUFF_SIZE size)
{
	if (size <= getLength())
	{
		memcpy(_buff, buff + offset, size);
		offset += size;
	}
	else if (size == 0)
	{
		return;
	}
	else
	{
		throw (std::string("Not enough memory in PackageBuffer"));
	}
}

PackageBuffer::PackageBuffer(const uint8_t *_buff, BUFF_SIZE _length) :
    Buffer(nullptr, 0)
{
	buff = new uint8_t[_length + 1];
    length = _length;
    memcpy(buff, _buff, _length);
	offset = 0;
}

PackageBuffer::~PackageBuffer()
{
}

void PackageBuffer::concatBuff(uint8_t *_buff, BUFF_SIZE _length)
{
	if (_length == 0)
		return;

	uint8_t *newBuff = new uint8_t[_length + length + 1];
	if (length > 0)
		memcpy(newBuff, buff, length);

	memcpy(newBuff + length, _buff, _length);

	delete[] buff;
	buff = newBuff;
	length += _length;
}

PackageBuffer *PackageBuffer::copyToNewAndClear()
{
	PackageBuffer *newBuff = new PackageBuffer(buff, length);

	this->buff = nullptr;
	this->length = 0;

	return newBuff;
}
