/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
\class
\brief
*/
class Key
{
    uint8_t * buff;
    const size_t key_length;
public:
    size_t getKeyLength() const
    {
        return key_length;
    }

    Key(uint8_t * buff, size_t _key_length, size_t precise_key_length):
        buff(buff), key_length(_key_length)
    {
        if (precise_key_length != _key_length)
            throw("key length and key precise length is not the same!");
    }

    virtual ~Key()
    {
        delete[] buff;
    }

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    const uint8_t * getBuff()const
    {
        return buff;
    }
};


class Key64 : Key
{
public:
    Key64(uint8_t * buff, size_t _key_length):
        Key(buff, _key_length, 64)
    {}
};
class Key128 : Key
{
public:
    Key128(uint8_t * buff, size_t _key_length):
        Key(buff, _key_length, 128)
    {}
};
class Key256 : Key
{
public:
    Key256(uint8_t * buff, size_t _key_length):
        Key(buff, _key_length, 256)
    {}
};
class Key512 : Key
{
public:
    Key512(uint8_t * buff, size_t _key_length):
        Key(buff, _key_length, 512)
    {}
};
class Key1024 : Key
{
public:
    Key1024(uint8_t * buff, size_t _key_length):
        Key(buff, _key_length, 1024)
    {}
};
class Key2048 : Key
{
public:
    Key2048(uint8_t * buff, size_t _key_length):
        Key(buff, _key_length, 2048)
    {}
};
