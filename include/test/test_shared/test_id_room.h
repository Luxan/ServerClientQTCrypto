#pragma once

#include <QtTest/QtTest>

#include "../../../include/shared/id_room.h"
#include "../../../include/test/test_base.h"

class Test_RoomID : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_RoomID, TestBase)
private slots:
    virtual void setup();
    virtual void constructor();
    void operators();
    virtual void tearDown();
};
