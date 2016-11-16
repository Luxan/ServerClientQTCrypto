#pragma once

#include <QtTest/QtTest>

#include "../../../include/shared/package_buffer.h"
#include "../../../include/test/test_base.h"

class Test_PackageBuffer : public TestBase
{
	Q_OBJECT
    CLASSTEST(Test_PackageBuffer, TestBase)
private:
	PackageBuffer *b1, *b2, *b3, *b4, *b5, *b6;
	uint8_t * tab;
    BUFF_SIZE size;
private slots:
    virtual void setup();
    virtual void constructor();
    void getLength();
    void fillBuffer();
    void concatBuff();
    void getPointerToBuffer();
    void copyToNewAndClear();
    void tearDown();
};



