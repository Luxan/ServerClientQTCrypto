#pragma once

#include "buffer.h"

class PackageBuffer : public Buffer
{
private:
	uint8_t offset;

public:
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
	virtual size_t getLength() const;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
	virtual uint8_t * getPointerToBuffer() const;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
	void fillBuffer(uint8_t *_buff, size_t size);

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
	PackageBuffer(const uint8_t *_buff, uint8_t _length);

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
	virtual ~PackageBuffer();

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
	void concatBuff(uint8_t *_buff, uint8_t _length);

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
	PackageBuffer *copyToNewAndClear();
};