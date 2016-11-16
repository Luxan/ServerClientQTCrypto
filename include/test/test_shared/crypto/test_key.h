#pragma once

#include <QtTest/QtTest>
#include <QTest>

#include "../../../include/shared/crypto/key.h"
#include "../../../include/test/test_base.h"

class Test_Key : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_Key, TestBase)
private:
    Key256 * k1, *k2;
    uint8_t tab[256];
    Buffer * b;
private slots:
    virtual void setup()
    {
        for (int i = 0; i < 256; i++)
        {
            tab[i] = i;
        }
        b = new Buffer(tab, sizeof(tab));
    }

    virtual void constructor()
    {
        k1 = new Key256(tab, Key::EncryptionKeyType::AES);
        k2 = new Key256(tab, Key::EncryptionKeyType::RSAPrivate);

        QVERIFY(k1 == k2);
        QVERIFY(k1 == b);
    }

    virtual void tearDown()
    {
        delete k1;
        delete k2;
        delete b;
    }
};
