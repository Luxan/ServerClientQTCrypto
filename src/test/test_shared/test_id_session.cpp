#include "../../../include/test/test_shared/test_id_session.h"
void Test_SessionID::setup(){}
void Test_SessionID::constructor()
{
    SessionID id1(1);
    SessionID id2(id1);
    SessionID id3("2");
    SessionID id4, id5, id6;

    QCOMPARE(id1.operator uint32_t(), (uint32_t)1);
    QVERIFY(id2.operator ==(id1));
    QCOMPARE(id3.operator uint32_t(), (uint32_t)2);
    QCOMPARE(id4.operator uint32_t(), (uint32_t)12);
    QCOMPARE(id5.operator uint32_t(), (uint32_t)13);
    QCOMPARE(id6.operator uint32_t(), (uint32_t)14);
}
void Test_SessionID::operators()
{
    SessionID id1(1);
    SessionID id2("1");
    SessionID id3("2");
    SessionID id4 = 5;

    QVERIFY(id2.operator ==(id1));
    QVERIFY(id2.operator ==(1));
    QVERIFY(!id2.operator >(id1));
    QVERIFY(id3.operator >(id1));
    QVERIFY(id2.operator <(id3));
    QCOMPARE(id2.operator uint32_t(), (uint32_t)1);
    QCOMPARE(id4.operator uint32_t(), (uint32_t)5);
}
void Test_SessionID::tearDown(){}
