#pragma once

#include "../../../include/shared/buffer.h"
#include "../../../include/test/test_base.h"

#include <QtTest/QtTest>
#include <QTest>

class Test_Buffer : public TestBase
{
    Q_OBJECT
private:
    Buffer *b1;
	uint8_t * tab;
    BUFF_SIZE size;
private slots:
	virtual void setup()
	{
        size = rand() % 100;
		tab = new uint8_t[size];
        for (BUFF_SIZE i = 0; i < size; i++)
		{
			tab[i] = i;
		}
        b1 = new Buffer(tab, size);
	}

	virtual void constructor()
	{
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
	
	void getLength()
	{				
        QCOMPARE(b1->getLength(), size);
	}

	void getPointerToBuffer()
	{
        QCOMPARE(*b1->getPointerToBuffer(), *tab);
    }

    virtual void tearDown()
    {
        delete[] tab;

        delete b1;
    }
};
