/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../buffer.h"

/**
\class
\brief
*/
class Key : public Buffer
{
public:
    enum EncryptionKeyType
    {
        RSAPublic,
        RSAPrivate,
        AES,
        Undefined
    };
private:
    EncryptionKeyType type;
public:
    Key(uint8_t * buff, size_t _key_length, EncryptionKeyType type):
        Buffer(buff, _key_length), type(type)
    {
        try
        {
            uint8_t a = *(buff + _key_length);
        }
        catch(const std::out_of_range& oor)
        {
            throw("Not enought data in buffer. Key length:" + std::to_string(length) + " error: " + oor.what());
        }
    }

    Key(uint8_t * buff, size_t _key_length):
        Buffer(buff, _key_length), type(Undefined)
    {
        try
        {
            uint8_t a = *(buff + _key_length);
        }
        catch(const std::out_of_range& oor)
        {
            throw("Not enought data in buffer. Key length:" + std::to_string(length) + " error: " + oor.what());
        }
    }
};


class Key64 : public Key
{
public:
    Key64(uint8_t * buff, EncryptionKeyType type):
        Key(buff, 64, type)
    {}
};
class Key128 : public Key
{
public:
    Key128(uint8_t * buff, EncryptionKeyType type):
        Key(buff, 128, type)
    {}
};
class Key256 : public Key
{
public:
    Key256(uint8_t * buff, EncryptionKeyType type):
        Key(buff, 256, type)
    {}
};
class Key512 : public Key
{
public:
    Key512(uint8_t * buff, EncryptionKeyType type):
        Key(buff, 512, type)
    {}
};
class Key1024 : public Key
{
public:
    Key1024(uint8_t * buff, EncryptionKeyType type):
        Key(buff, 1024, type)
    {}
};
class Key2048 : public Key
{
public:
    Key2048(uint8_t * buff, EncryptionKeyType type):
        Key(buff, 2048, type)
    {}
};
