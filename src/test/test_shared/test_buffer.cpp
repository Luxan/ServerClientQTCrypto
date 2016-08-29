#include "../../../include/shared/buffer.h"
#include "../../../include/test/test_base.h"

class Test_Buffer : public TestBase
{
    Q_OBJECT
private:
	Buffer *b1, *b2, *b3;
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
		b1 = new Buffer(tab, size);
		b2 = new Buffer(tab, size);
		b3 = new Buffer(tab, size);
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

	virtual void tierDown()
	{
		delete[] tab;
		
		delete b1;
		delete b2;
		delete b3;

		showTimeElapsed();
	}
};