#include "../../../include/shared/buffer_spitter.h"
#include "../../../include/shared/package_buffer.h"
#include "../../../include/test/test_base.h"

#include <list>

class Test_BufferSpitter : public TestBase
{
	Q_OBJECT
private:
	bufferSpitter *b1, *b2, *b3, *b4, *b5, *b6;
	uint8_t * tab, *tab2;
private slots:

	virtual void setup()
	{
		size_t size = rand();
		tab = new uint8_t[size];
		for (size_t i = 0; i < size; i++)
		{
			tab[i] = i;
		}
		b1 = new bufferSpitter(tab, size);
		b2 = new bufferSpitter(tab, size);
		b3 = new bufferSpitter(tab, size);
		
		int x = 10;
		size_t ss = 65;
		tab2 = new uint8_t[ss];
		while (x > 0)
		{
			int z = x--;
			while (z > 0)
			{
				tab2[] = 
			}
		}


		for (size_t i = 1; i <= x; i++)
		{
			tab2[]
			for (size_t k = 1; k <= i; k++)
			{

			}
		}

		b4 = new bufferSpitter(tab, size);
	}

	virtual void constructor()
	{
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
	}

	void getPointerToBuffer()
	{
		QVERIFY(b3->getPointerToBuffer() == tab);
	}

	void splitBufferIntoList()
	{
		std::list<PackageBuffer *> list;
		PackageBuffer *incompletePackageBuffer;
		size_t &incompletePackageFullLength;
	}

	void tierDown()
	{
		delete[] tab;
		delete[] tab2;

		delete b1;
		delete b2;
		delete b3;
		delete b4;

		showTimeElapsed();
	}
};



