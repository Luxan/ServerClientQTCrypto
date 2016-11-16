#pragma once

#include <QtTest/QtTest>
#include <QTest>

#include "../../../include/shared/crypto/crypto.h"
#include "../../../include/test/test_base.h"

class Test_Crypto : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_Crypto, TestBase)
private:
private slots:
    virtual void setup(){}

    virtual void constructor(){}

    void createRandomBuffer()
    {
        uint8_t * tab1 = CryptographyBase::createRandomBuffer(100);
        uint8_t * tab2 = CryptographyBase::createRandomBuffer(100);
        QVERIFY(std::equal(tab1, tab1 + 100, tab2));
    }

    virtual void tearDown(){}
};
