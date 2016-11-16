#pragma once

#include <QtTest/QtTest>

#include "../../../include/shared/user_credentials.h"
#include "../../../include/shared/id_client.h"
#include "../../../include/test/test_base.h"

class Test_UserCredentials : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_UserCredentials, TestBase)
private:
    UserCredentials * credentials;
    Hash * hash;
    std::string login;
    size_t size;
    Hash * h;
    uint32_t passwordItCount;
    uint64_t salt;
    ClientID id;
private slots:
    virtual void setup();

    virtual void constructor();

    void isCorrectPassword();

    virtual void tearDown();
};
