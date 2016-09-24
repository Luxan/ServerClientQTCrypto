#pragma once

#include <QtTest/QtTest>

#include "../../../include/shared/id_room.h"
#include "../../../include/test/test_base.h"

class Test_RoomID : public TestBase
{
    Q_OBJECT
private slots:
    virtual void setup(){}
    virtual void constructor()
    {
        RoomID id1(1);
        RoomID id2(id1);
        RoomID id3("2");
        RoomID id4, id5, id6;

        QCOMPARE(id1.operator uint32_t(), (uint32_t)1);
        QVERIFY(id2.operator ==(id1));
        QCOMPARE(id3.operator uint32_t(), (uint32_t)2);
        QCOMPARE(id4.operator uint32_t(), (uint32_t)3);
        QCOMPARE(id5.operator uint32_t(), (uint32_t)4);
        QCOMPARE(id6.operator uint32_t(), (uint32_t)5);
    }

    void operators()
    {
        RoomID id1(1);
        RoomID id2("1");
        RoomID id3("2");
        RoomID id4;
        id4 = 5;

        QVERIFY(id2.operator ==(id1));
        QVERIFY(!id2.operator >(id1));
        QVERIFY(id3.operator >(id1));
        QVERIFY(id2.operator <(id3));
        QCOMPARE(id2.operator uint32_t(), (uint32_t)1);
        QCOMPARE(id4.operator uint32_t(), (uint32_t)5);
    }
    virtual void tearDown(){}
};
