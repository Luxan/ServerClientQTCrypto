#include "../../../include/test/test_shared/test_buffer_spitter.h"

void Test_BufferSpitter::setup()
{
    size = rand() % 100;
    tab = new uint8_t[size];
    for (size_t i = 0; i < size; i++)
    {
        tab[i] = i;
    }
    b1 = new BufferSpitter(tab, size);

    uint8_t tab2[100];
    size_t offset = 0;
    uint8_t tab3[100];
    size_t offset2 = 0;

    for (BUFF_SIZE i = 0; i <= 6; i++)
    {
        if (i > 5)
        {
            memcpy(tab3 + offset2, &i, sizeof(BUFF_SIZE));
            offset2 += sizeof(BUFF_SIZE);
        }
        else
        {
            memcpy(tab2 + offset, &i, sizeof(BUFF_SIZE));
            offset += sizeof(BUFF_SIZE);
        }
        for (int8_t x = 0; x < i; x++)
        {
            if ((x > 2 && i == 5) || i > 5)
            {
                memcpy(tab3 + offset2, &x, sizeof(int8_t));
                offset2 += sizeof(int8_t);
            }
            else
            {
                memcpy(tab2 + offset, &x, sizeof(int8_t));
                offset += sizeof(int8_t);
            }
        }
    }

    b2 = new BufferSpitter(tab2, offset);

    b3 = new BufferSpitter(tab3, offset2);
}

void Test_BufferSpitter::constructor()
{
    for (size_t i = 0; i < size; i++)
    {
        if (*(b1->getPointerToBuffer() + i) != i)
        {
            QCOMPARE(*(b1->getPointerToBuffer() + i), (uint8_t)i);
            qDebug("Consrtucted buffer has incorrect Data");
            break;
        }
    }
}

void Test_BufferSpitter::getLength()
{
    QCOMPARE(b1->getLength(), size);
}

void Test_BufferSpitter::getPointerToBuffer()
{
    QCOMPARE(*b1->getPointerToBuffer(), *tab);
}

void Test_BufferSpitter::splitBufferIntoList()
{
    PackageBuffer incompletePackageBuffer(nullptr, 0);
    BUFF_SIZE incompletePackageFullLength = 0;

    b2->splitBufferIntoList(list, &incompletePackageBuffer, incompletePackageFullLength);

    QCOMPARE(incompletePackageBuffer.getLength(), (BUFF_SIZE)3);
    QCOMPARE(incompletePackageFullLength, (BUFF_SIZE)5);
    QCOMPARE(list.size(), (size_t)4);

    b3->splitBufferIntoList(list, &incompletePackageBuffer, incompletePackageFullLength);

    QCOMPARE(incompletePackageBuffer.getLength(), (BUFF_SIZE)0);
    QCOMPARE(incompletePackageFullLength, (BUFF_SIZE)0);
    QCOMPARE(list.size(), (size_t)6);

    for (PackageBuffer * p : list)
    {
        for (int i = 0; i < p->getLength(); i++)
        {
            QCOMPARE(*(p->getPointerToBuffer() + i), (uint8_t)i);
        }
    }
}
void Test_BufferSpitter::tearDown()
{
    delete[] tab;

    delete b1;
    delete b2;
    delete b3;

    for (PackageBuffer * p : list)
    {
        delete p;
    }
}
