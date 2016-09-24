/**
\author Sergey Gorokh (ESEGORO)
*/

#include <stdint.h>
#include <string>
#include <stdio.h>

#include "../../../include/shared/package.h"
#include "../../../include/shared/package_buffer.h"
#include "../../../include/test/test_base.h"
 
class Test_Package : public TestBase
{
	Q_OBJECT
private:
	Package *b;
private slots:
	virtual void setup()
	{
		b = new Package();
	}

	void thowNotEnoughSizeException()
	{
		for (int i = 0; int i < 100; i++)
		{
			size_t gotSize = rand();
			size_t expectedSize = rand();

			bool shouldThrow = gotSize < expectedSize;

			try
			{
				b->thowNotEnoughSizeException(gotSize, expectedSize);
			}
			catch (std::string error)
			{
				QVERIFY(!shouldThrow);
			}
		}		
	}

	virtual void tierDown()
	{
		delete b;

		showTimeElapsed();
	}
};


class Test_PackageDynamicSize : public TestBase
{
	Q_OBJECT
private:
	PackageDynamicSize *b;
	PackageBuffer * p;
	private slots:
	virtual void setup()
	{
		p = new PackageBuffer(new uint8_t[100], 100);
		b = new PackageRequestAutocomplete(p);
	}

	void checkIfEnoughSize()
	{
		for (int i = 0; int i < 100; i++)
		{
			size_t expectedSize = rand() % 200;

			bool shouldThrow = p->getLength() < expectedSize;

			try
			{
				b->thowNotEnoughSizeException(p->getLength(), expectedSize);
			}
			catch (std::string error)
			{
				QVERIFY(!shouldThrow);
			}
		}
	}

	virtual void tierDown()
	{
		delete b;
		delete p;

		showTimeElapsed();
	}
};


class Test_PackageMultiPackage : public TestBase
{
	Q_OBJECT
private:
	PackageMultiPackage *b;
	PackageBuffer * p;
	PackageBuffer * p2;
private slots:
	virtual void setup()
	{
		p = new PackageBuffer(new uint8_t[100], 100);
		p2 = new PackageBuffer(nullptr, 0);
	}

	void constructor()
	{
		b = nullptr;
		try
		{
			b = new PackageResponseAutocomplete(p2);
			QVERIFY(b! == nullptr);
			delete b;
		}
		catch (std::string error)
		{}

		b = nullptr;

		try
		{
			b = new PackageResponseAutocomplete(p1);
			QVERIFY(b != nullptr);
			delete b;
		}
		catch (std::string error)
		{
			QVERIFY(b != nullptr);
			delete b;
		}
	}

	virtual void tierDown()
	{
		delete p;
		delete p2;

		showTimeElapsed();
	}
};


class Test_PackageRequestAutocomplete : public TestBase
{
	Q_OBJECT
private:
	Package *b;
	private slots:
	virtual void setup()
	{
		b = new Package();
	}

	void thowNotEnoughSizeException()
	{
		for (int i = 0; int i < 100; i++)
		{
			size_t gotSize = rand();
			size_t expectedSize = rand();

			bool shouldThrow = gotSize < expectedSize;

			try
			{
				b->thowNotEnoughSizeException(gotSize, expectedSize);
			}
			catch (std::string error)
			{
				QVERIFY(!shouldThrow);
			}
		}
	}


	virtual size_t realSize()const
	{
		return buff->getLength();
	}

	virtual size_t minSize()const
	{
		return buff->getLength();
	}

	virtual void tierDown()
	{
		delete b;

		showTimeElapsed();
	}
};

/**
\struct
\brief
*/
struct PackageResponseAutocomplete : PackageMultiPackage
{
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageResponseAutocomplete(PackageBuffer *buf):
        PackageMultiPackage(buf)
    {}

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual size_t realSize()const
    {
        return buff->getLength();
    }

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual size_t minSize()const
    {
        return buff->getLength();
    }

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~PackageResponseAutocomplete() {}
};
#endif // PACKAGE_H

