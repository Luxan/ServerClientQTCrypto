#include "../../../include/test/test_shared/test_buffer.h"


void Test_Buffer::setup()
{
    size = rand() % 100;
    tab = new uint8_t[size];
    tab2 = new uint8_t[size];
    for (BUFF_SIZE i = 0; i < size; i++)
    {
        tab[i] = i;
        tab2[i] = i + 1;
    }
}

void Test_Buffer::constructor()
{
    b1 = new Buffer(tab, size);
    b2 = new Buffer(tab, size);
    b3 = new Buffer();
    b4 = new Buffer(tab2, size);
    for (BUFF_SIZE i = 0; i < size; i++)
    {
        if (*(b1->getPointerToBuffer() + i) != i)
        {
            QCOMPARE(*(b1->getPointerToBuffer() + i), (uint8_t)i);
            qDebug("Consrtucted buffer has incorrect Data");
            break;
        }
    }
}

void Test_Buffer::getLength()
{
    QCOMPARE(b1->getLength(), size);
}

void Test_Buffer::getPointerToBuffer()
{
    QCOMPARE(*b1->getPointerToBuffer(), *tab);
}

void Test_Buffer::operators()
{
    QVERIFY(*b1 == *b2);
    QVERIFY(*b1 != *b3);
    QVERIFY(*b1 != *b4);
}

void Test_Buffer::tearDown()
{
    delete[] tab;
    delete[] tab2;

    delete b1;
    delete b2;
    delete b3;
    delete b4;
}
