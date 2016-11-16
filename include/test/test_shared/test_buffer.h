#pragma once

#include <QtTest/QtTest>
#include <QTest>

#include "../../../include/shared/buffer.h"
#include "../../../include/test/test_base.h"

class Test_Buffer : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_Buffer, TestBase)
private:
    Buffer *b1, *b2, *b3, *b4;
    uint8_t * tab, *tab2;
    BUFF_SIZE size;
private slots:
    virtual void setup();

    virtual void constructor();
	
    void getLength();

    void getPointerToBuffer();

    void operators();

    virtual void tearDown();
};
