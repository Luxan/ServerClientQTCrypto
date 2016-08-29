#include "../../../include/shared/package_buffer.h"
#include "../../../include/test/test_base.h"

class Test_PackageBuffer : public TestBase
{
	Q_OBJECT
private:
	PackageBuffer *b1, *b2, *b3, *b4, *b5, *b6;
	uint8_t * tab;
private slots:

	virtual void setup()
	{
		size_t size = rand();
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

	virtual void constructor()
	{
		b1 = new PackageBuffer(tab, size);

		for (size_t i = 0; i < size; i++)
		{
			if (b2->getPointerToBuffer() + i != i)
			{
				QVERIFY(b2->getPointerToBuffer() + i == i);
				qDebug("Consrtucted buffer has incorrect Data");
				break;
			}
		}
	}

	void getLength()
	{
		QVERIFY(b2->getLength() == size);

		uint8_t x[size - 1];

		b2->fillBuffer(x, size - 1);
		QVERIFY(b2->getLength() == 1);
	}

	void fillBuffer()
	{
		uint8_t x1[5];
		uint8_t x2[10];
		uint8_t x3[15];
		uint8_t x4[30];
		uint8_t x5[50];

		b3->fillBuffer(x1, 5);
		QVERIFY(b3->getLength() == size - 5);
		for (int i = 0; i < 5; i++)
		{
			QVERIFY(*(x1 + i) == i);
		}

		b3->fillBuffer(x2, 10);
		QVERIFY(b3->getLength() == size - 15);
		for (int i = 0; i < 5; i++)
		{
			QVERIFY(*(x1 + i) == 5 + i);
		}

		b3->fillBuffer(x3, 15);
		QVERIFY(b3->getLength() == size - 30);
		for (int i = 0; i < 5; i++)
		{
			QVERIFY(*(x1 + i) == 15 + i);
		}

		b3->fillBuffer(x4, 30);
		QVERIFY(b3->getLength() == size - 60);
		for (int i = 0; i < 5; i++)
		{
			QVERIFY(*(x1 + i) == 30 + i);
		}

		b3->fillBuffer(x5, 50);
		QVERIFY(b3->getLength() == size - 110);
		for (int i = 0; i < 5; i++)
		{
			QVERIFY(*(x1 + i) == 60 + i);
		}
	}

	void concatBuff()
	{
		b4->concatBuff(tab, size);
		for(int i = 0; i < 2*size; i++)
		{
			if (i < size)
				QVERIFY(*(b4->getPointerToBuffer() + i) == i);
			else
				QVERIFY(*(b4->getPointerToBuffer() + i) == i - size);
		}
	}
	
	void getPointerToBuffer()
	{
		QVERIFY(b5->getPointerToBuffer() == tab);

		uint8_t x1[5];
		
		b5->fillBuffer(x1, 5);
		QVERIFY(b5->getPointerToBuffer() == tab + 5);
	}

	void copyToNewAndClear()
	{
		PackageBuffer * newOne = b6->copyToNewAndClear();

		QVERIFY(b6->getPointerToBuffer() == nullptr);
		QVERIFY(b6->getLength() == 0);

		for (size_t i = 0; i < size; i++)
		{
			if (newOne->getPointerToBuffer() + i != i)
			{
				QVERIFY(newOne->getPointerToBuffer() + i == i);
				qDebug("Consrtucted buffer has incorrect Data");
				break;
			}
		}
	}

	void tierDown()
	{
		delete[] tab;

		delete b1;
		delete b2;
		delete b3;
		delete b4;
		delete b5;
		delete b6;

		showTimeElapsed();
	}
};



