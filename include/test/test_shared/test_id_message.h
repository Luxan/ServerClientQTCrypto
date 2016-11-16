#pragma once

#include <QtTest/QtTest>

#include "../../../include/shared/id_message.h"
#include "../../../include/test/test_base.h"

class Test_MessageID : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_MessageID, TestBase)
private slots:
    virtual void setup();
    virtual void constructor();
    void operators();
    virtual void tearDown();
};
