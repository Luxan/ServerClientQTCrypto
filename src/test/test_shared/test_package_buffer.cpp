#include "../../../include/test/test_shared/test_package_buffer.h"
void Test_PackageBuffer::setup()
{
    size = (rand() % 100) + 7;
    tab = new uint8_t[size];
    for (size_t i = 0; i < size; i++)
    {
        tab[i] = i;
    }
    b1 = new PackageBuffer(tab, size);
    b2 = new PackageBuffer(tab, size);
    b3 = new PackageBuffer(tab, size);
    b4 = new PackageBuffer(tab, size);
    b5 = new PackageBuffer(tab, size);
    b6 = new PackageBuffer(tab, size);
}

void Test_PackageBuffer::constructor()
{
    b1 = new PackageBuffer(tab, size);

    for (BUFF_SIZE i = 0; i < size; i++)
    {
        if (*(b2->getPointerToBuffer() + i) != i)
        {
            QCOMPARE(*(b2->getPointerToBuffer() + i), (uint8_t)i);
            qDebug("Consrtucted buffer has incorrect Data");
            break;
        }
    }
}

void Test_PackageBuffer::getLength()
{
    QCOMPARE(b2->getLength(), size);

    uint8_t x[size - 1];

    b2->fillBuffer(x, size - 1);
    QCOMPARE(b2->getLength(), (BUFF_SIZE)1);
}

void Test_PackageBuffer::fillBuffer()
{
    uint8_t x1[5];
    uint8_t x2[size - 5];

    b3->fillBuffer(x1, 5);
    QCOMPARE(b3->getLength(), (BUFF_SIZE)(size - 5));
    for (int i = 0; i < 5; i++)
    {
        QCOMPARE(*(x1 + i), (uint8_t)i);
    }

    b3->fillBuffer(x2, size - 5);
    QCOMPARE(b3->getLength(), (BUFF_SIZE)0);
    for (int i = 5; i < size; i++)
    {
        QCOMPARE(*(x2 + i - 5), (uint8_t)i);
    }
}

void Test_PackageBuffer::concatBuff()
{
    b4->concatBuff(tab, size);
    for(size_t i = 0; i < 2*size; i++)
    {
        if (i < size)
            QCOMPARE(*(b4->getPointerToBuffer() + i), (uint8_t)i);
        else
            QCOMPARE(*(b4->getPointerToBuffer() + i), (uint8_t)(i - size));
    }
}

void Test_PackageBuffer::getPointerToBuffer()
{
    QCOMPARE(*b5->getPointerToBuffer(), *tab);

    uint8_t x1[5];

    b5->fillBuffer(x1, 5);
    QCOMPARE(*b5->getPointerToBuffer(), *(tab + 5));
}

void Test_PackageBuffer::copyToNewAndClear()
{
    PackageBuffer * newOne = b6->copyToNewAndClear();

    QVERIFY(b6->getPointerToBuffer() == nullptr);
    QCOMPARE(b6->getLength(), (BUFF_SIZE)0);

    for (size_t i = 0; i < size; i++)
    {
        if (*(newOne->getPointerToBuffer() + i) != i)
        {
            QCOMPARE(*(newOne->getPointerToBuffer() + i), (uint8_t)i);
            qDebug("Consrtucted buffer has incorrect Data");
            break;
        }
    }
}
void Test_PackageBuffer::tearDown()
{
    delete[] tab;

    delete b1;
    delete b2;
    delete b3;
    delete b4;
    delete b5;
    delete b6;
}

