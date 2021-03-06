#include "../../../include/test/test_shared/test_id_client.h"

void Test_ClientID::setup(){}

void Test_ClientID::constructor()
{
    ClientID id1(1);
    ClientID id2(id1);
    ClientID id3("2");
    ClientID id4, id5, id6;

    QCOMPARE(id1.operator uint32_t(), (uint32_t)1);
    QVERIFY(id2.operator ==(id1));
    QCOMPARE(id3.operator uint32_t(), (uint32_t)2);
    QCOMPARE(id4.operator uint32_t(), (uint32_t)3);
    QCOMPARE(id5.operator uint32_t(), (uint32_t)4);
    QCOMPARE(id6.operator uint32_t(), (uint32_t)5);
}

void Test_ClientID::operators()
{
    ClientID id1(1);
    ClientID id2("1");
    ClientID id3("2");
    ClientID id4 = 5;

    QVERIFY(id2.operator ==(id1));
    QVERIFY(id2.operator ==(1));
    QVERIFY(!id2.operator >(id1));
    QVERIFY(id3.operator >(id1));
    QVERIFY(id2.operator <(id3));
    QCOMPARE(id2.operator uint32_t(), (uint32_t)1);
    QCOMPARE(id4.operator uint32_t(), (uint32_t)5);
}

void Test_ClientID::tearDown(){}
