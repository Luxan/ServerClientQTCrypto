#pragma once

#include <QtTest/QtTest>

#include "../../../include/shared/buffer_spitter.h"
#include "../../../include/shared/package_buffer.h"
#include "../../../include/test/test_base.h"

#include <list>

class Test_BufferSpitter : public TestBase
{
	Q_OBJECT
private:
    bufferSpitter *b1;
    bufferSpitter *b2;
    bufferSpitter *b3;
    uint8_t * tab;
    BUFF_SIZE size;
    std::list<PackageBuffer *> list;

private slots:
    virtual void setup()
    {
        size = rand() % 100;
        tab = new uint8_t[size];
        for (size_t i = 0; i < size; i++)
        {
            tab[i] = i;
        }
        b1 = new bufferSpitter(tab, size);

        uint8_t tab2[100];
        size_t offset = 0;
        uint8_t tab3[100];
        size_t offset2 = 0;

        for (BUFF_SIZE i = 0; i != 5; i++)
        {
            memcpy(tab2 + offset, &i, sizeof(BUFF_SIZE));
            offset += sizeof(BUFF_SIZE);
            for (int8_t x = 0; x < i; x++)
            {
                memcpy(tab2 + offset, &x, sizeof(int8_t));
                offset += sizeof(int8_t);
                if (x > 4)
                {
                    memcpy(tab3 + offset, &x, sizeof(int8_t));
                    offset2 += sizeof(int8_t);
                }

            }
        }

        b2 = new bufferSpitter(tab2, offset);

        for (BUFF_SIZE i = 6; i != 8; i++)
        {
            memcpy(tab3 + offset2, &i, sizeof(BUFF_SIZE));
            offset2 += sizeof(BUFF_SIZE);
            for (int8_t x = 0; x < i; x++)
            {
                memcpy(tab3 + offset2, &x, sizeof(int8_t));
                offset2 += sizeof(int8_t);
            }
        }

        b3 = new bufferSpitter(tab3, offset2);
   }

	virtual void constructor()
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

	void getLength()
	{
        QCOMPARE(b1->getLength(), size);
	}

	void getPointerToBuffer()
	{
        QCOMPARE(*b1->getPointerToBuffer(), *tab);
	}

	void splitBufferIntoList()
    {
        PackageBuffer incompletePackageBuffer(nullptr, 0);
        BUFF_SIZE incompletePackageFullLength = 0;

        b2->splitBufferIntoList(list, &incompletePackageBuffer, incompletePackageFullLength);

        QCOMPARE(incompletePackageBuffer.getLength(), (BUFF_SIZE)4);
        QCOMPARE(incompletePackageFullLength, (BUFF_SIZE)5);
        QCOMPARE(list.size(), (size_t)3);

        b3->splitBufferIntoList(list, &incompletePackageBuffer, incompletePackageFullLength);

        QCOMPARE(incompletePackageBuffer.getLength(), (BUFF_SIZE)0);
        QCOMPARE(incompletePackageFullLength, (BUFF_SIZE)0);
        QCOMPARE(list.size(), (size_t)7);

        for (PackageBuffer * p : list)
        {
            for (int i = 0; i < *p->getPointerToBuffer(); i++)
            {
                QCOMPARE(*(p->getPointerToBuffer() + i), (uint8_t)i);
            }
        }
	}
    virtual void tearDown()
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
};

