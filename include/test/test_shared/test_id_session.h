#pragma once

#include <QtTest/QtTest>

#include "../../../include/shared/id_session.h"
#include "../../../include/test/test_base.h"

class Test_SessionID : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_SessionID, TestBase)
private slots:
    virtual void setup();
    virtual void constructor();
    void operators();
    virtual void tearDown();
};
