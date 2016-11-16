#pragma once

#include <QtTest/QtTest>

#include "../../../include/shared/id_client.h"
#include "../../../include/test/test_base.h"

class Test_ClientID : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_ClientID, TestBase)
private slots:
    virtual void setup();
    virtual void constructor();
    void operators();
    virtual void tearDown();
};
