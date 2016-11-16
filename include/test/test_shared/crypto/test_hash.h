#pragma once

#include <QtTest/QtTest>
#include <QTest>

#include "../../../include/shared/crypto/hash.h"
#include "../../../include/test/test_base.h"

class Test_Hash : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_Hash, TestBase)
private:
    Hash * h1, *h2, *h3, *h4;
    uint8_t tab1[100];
    uint8_t tab2[100];
    uint8_t tab3[100];
private slots:
    virtual void setup()
    {
        for(int i = 0; i < 100; i++)
        {
            tab1[i] = i;
            tab2[i] = i;
            tab3[99 - i] = i;
        }
    }

    virtual void constructor()
    {
        h1 = new Hash(tab1, sizeof(tab1));
        h2 = new Hash(tab2, sizeof(tab2));
        h3 = new Hash(tab3, sizeof(tab3));
        h4 = new Hash();
    }

    void operators()
    {
        QVERIFY(h1 == h2);
        QVERIFY(h1 != h3);
        QVERIFY(h3 != h4);
    }

    virtual void tearDown()
    {
        delete h1;
        delete h2;
        delete h3;
        delete h4;
    }
};
