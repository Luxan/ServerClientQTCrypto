/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <stdint.h>

/**
\class
\brief
*/
class Key
{
    const size_t key_length;
    uint8_t buff[key_length];
public:
    const size_t getKeyLength() const
    {
        return key_length;
    }

    Key(size_t _key_length):
        key_length(_key_length)
    {}

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
    Key64():
        Key(64)
    {}
};
class Key128 : Key
{
public:
    Key128():
        Key(128)
    {}
};
class Key256 : Key
{
public:
    Key256():
        Key(256)
    {}
};
class Key512 : Key
{
public:
    Key512():
        Key(512)
    {}
};
class Key1024 : Key
{
public:
    Key1024():
        Key(1024)
    {}
};
class Key2048 : Key
{
public:
    Key2048():
        Key(2048)
    {}
};
