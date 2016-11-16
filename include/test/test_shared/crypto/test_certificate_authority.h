#pragma once

#include <QtTest/QtTest>
#include <QTest>

#include "../../../include/shared/crypto/certificate_authority.h"
#include "../../../include/test/test_base.h"

class Test_CertificateAuthority : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_CertificateAuthority, TestBase)
private:
        Buffer * certificateBuff;
        CertificateAuthority *certificate2;
    private slots:
        virtual void setup()
        {
            uint8_t tab[] = {0,1,2,3,4,5,6,7,8,9};
            certificateBuff = new Buffer(tab, sizeof(tab));
        }

        virtual void constructor()
        {
            certificate2 = new CertificateAuthority(certificateBuff);
            QCOMPARE(certificate2->getBuffer(), certificateBuff);
        }

        void getBuffer()
        {
            QCOMPARE(certificate2->getBuffer(), certificateBuff);
        }

        virtual void tearDown()
        {
            delete certificate2;
        }
};
