#pragma once

#include <QtTest/QtTest>
#include <QTest>

#include "../../../include/shared/crypto/hasher.h"
#include "../../../include/test/test_base.h"

class Test_Sha256Hasher : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_Sha256Hasher, TestBase)
private:
    uint8_t decoded[10] = {0,1,2,3,4,5,6,7,8,9};
    uint32_t ithercount1 = 10;
    uint32_t ithercount2 = 1000;

    Buffer * input1;
    PackageBuffer * input2;

    Hash * output1, *output2, *output3, *output4;
    Hash * h1, *h2;

private slots:
    virtual void setup()
    {
        uint8_t encoded1[Sha256Hasher::getInstance().getDigestSize()] = {0x8f,0x8e,0xaa,0xd1,0x6c,0xbf,0x87,0x22,0xa2,0x16,0x5b,0x66,0x0d,0x47,0xfc,0xfd,0x84,0x96,0xa4,0x1c,0x61,0x1d,0xa7,0x58,0xf3,0xbb,0x70,0xf8,0x09,0xf0,0x1e,0xe3};
        uint8_t encoded2[Sha256Hasher::getInstance().getDigestSize()] = {0x8f,0x8e,0xaa,0xd1,0x6c,0xbf,0x87,0x22,0xa2,0x16,0x5b,0x66,0x0d,0x47,0xfc,0xfd,0x84,0x96,0xa4,0x1c,0x61,0x1d,0xa7,0x58,0xf3,0xbb,0x70,0xf8,0x09,0xf0,0x1e,0xe3};
        input1 = new Buffer((uint8_t*)decoded, sizeof(decoded));
        input2 = new PackageBuffer((uint8_t*)decoded, sizeof(decoded));
        h1 = new Hash((uint8_t*)encoded1, sizeof(encoded1));
        h2 = new Hash((uint8_t*)encoded2, sizeof(encoded2));
    }

    virtual void constructor(){}

    void createHash()
    {
        output1 = Sha256Hasher::getInstance().createHash(input1, ithercount1);
        output2 = Sha256Hasher::getInstance().createHash(input1, ithercount2);
        output3 = Sha256Hasher::getInstance().createHash(input2, ithercount1);
        output4 = Sha256Hasher::getInstance().createHash(input2, ithercount2);

        QVERIFY(output1 == h1);
        QVERIFY(output3 == h1);
        QVERIFY(output2 == h2);
        QVERIFY(output4 == h2);
    }

    virtual void tearDown()
    {
        delete input1;
        delete input2;

        delete output1;
        delete output2;
        delete output3;
        delete output4;
        delete h1;
        delete h2;
    }
};
