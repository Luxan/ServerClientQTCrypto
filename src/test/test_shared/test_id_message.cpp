#include "../../../include/test/test_shared/test_id_message.h"
void Test_MessageID::setup(){}
void Test_MessageID::constructor()
{
    MessageID id1(1);
    MessageID id2(id1);
    MessageID id3("2");
    MessageID id4, id5, id6;

    QCOMPARE(id1.operator uint32_t(), (uint32_t)1);
    QVERIFY(id2.operator ==(id1));
    QCOMPARE(id3.operator uint32_t(), (uint32_t)2);
    QCOMPARE(id4.operator uint32_t(), (uint32_t)6);
    QCOMPARE(id5.operator uint32_t(), (uint32_t)7);
    QCOMPARE(id6.operator uint32_t(), (uint32_t)8);
}

void Test_MessageID::operators()
{
    MessageID id1(1);
    MessageID id2("1");
    MessageID id3("2");
    MessageID id4 = 5;

    QVERIFY(id2.operator ==(id1));
    QVERIFY(id2.operator ==(1));
    QVERIFY(!id2.operator >(id1));
    QVERIFY(id3.operator >(id1));
    QVERIFY(id2.operator <(id3));
    QCOMPARE(id2.operator uint32_t(), (uint32_t)1);
    QCOMPARE(id4.operator uint32_t(), (uint32_t)5);
}
void Test_MessageID::tearDown(){}
