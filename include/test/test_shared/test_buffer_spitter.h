#pragma once

#include <QtTest/QtTest>

#include "../../../include/shared/buffer_spitter.h"
#include "../../../include/shared/package_buffer.h"
#include "../../../include/test/test_base.h"

#include <list>

class Test_BufferSpitter : public TestBase
{
	Q_OBJECT
    CLASSTEST(Test_BufferSpitter, TestBase)
private:
    BufferSpitter *b1;
    BufferSpitter *b2;
    BufferSpitter *b3;
    uint8_t * tab;
    BUFF_SIZE size;
    std::list<PackageBuffer *> list;

private slots:
    virtual void setup();
    virtual void constructor();
    void getLength();
    void getPointerToBuffer();
    void splitBufferIntoList();
    void tearDown();
};

