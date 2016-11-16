#pragma once

#include <stdio.h>
#include <cryptopp/dh.h>
#include <cryptopp/osrng.h>
#include <cryptopp/nbtheory.h>

#include "../log.h"

class CryptographyBase
{
protected:
    CryptoPP::AutoSeededRandomPool rnd;
public:
    static uint8_t * createRandomBuffer(size_t size)
    {
        uint8_t * tab = new uint8_t[size];
        CryptoPP::OS_GenerateRandomBlock(false, tab, size);
        return tab;
    }

//    bool validateDH(CryptoPP::Integer p, CryptoPP::Integer g, CryptoPP::Integer q)
//    {
//        CryptoPP::DH dh;
//        dh.AccessGroupParameters().Initialize(p, q, g);

//        if(!dh.GetGroupParameters().ValidateGroup(rnd, 3))
//            throw "Failed to validate prime and generator";

//        p = dh.GetGroupParameters().GetModulus();

//        q = dh.GetGroupParameters().GetSubgroupOrder();

//        g = dh.GetGroupParameters().GetGenerator();

//        CryptoPP::Integer v = CryptoPP::ModularExponentiation(g, q, p);
//        if(v != CryptoPP::Integer::One())
//            throw "Failed to verify order of the subgroup";
//    }
};
