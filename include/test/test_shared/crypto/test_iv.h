#pragma once

#include <QtTest/QtTest>
#include <QTest>

#include "../../../include/shared/crypto/iv.h"
#include "../../../include/test/test_base.h"

class Test_Iv : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_Iv, TestBase)
private:
    IV1024 * iv1, *iv2;
private slots:
    virtual void setup(){}

    virtual void constructor()
    {
        iv1 = new IV1024();
        iv2 = new IV1024();
        QVERIFY(*iv1 != *iv2);
    }

    virtual void tearDown()
    {
        delete iv1;
        delete iv2;
    }
};
