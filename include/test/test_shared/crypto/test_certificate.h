#pragma once

#include <QtTest/QtTest>
#include <QTest>

#include "../../../include/shared/crypto/certificate.h"
#include "../../../include/test/test_base.h"

class Test_Certificate : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_Certificate, TestBase)
private:
    Buffer * certificateBuff;
    Certificate *certificate1, *certificate2;
private slots:
    virtual void setup()
    {
        uint8_t tab[] = {0,1,2,3,4,5,6,7,8,9};
        certificateBuff = new Buffer(tab, sizeof(tab));
    }

    virtual void constructor()
    {
        certificate1 = new Certificate(certificateBuff->getPointerToBuffer(), certificateBuff->getLength());
        QVERIFY(*certificate1->getBuffer() == *certificateBuff);
        certificate2 = new Certificate(certificateBuff);
        QCOMPARE(certificate2->getBuffer(), certificateBuff);
    }

    void getBuffer()
    {
        QCOMPARE(certificate2->getBuffer(), certificateBuff);
    }

    virtual void tearDown()
    {
        delete certificate1;
        delete certificate2;
    }
};
