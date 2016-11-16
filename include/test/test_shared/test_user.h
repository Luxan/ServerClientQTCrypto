#pragma once

#include <QtTest/QtTest>

#include "../../../include/shared/user.h"
#include "../../../include/shared/id_client.h"
#include "../../../include/test/test_base.h"

class Test_User : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_User, TestBase)
private:
    UserCredentials * credentials;
    UserRelations * relations;
    size_t size;
    User * u;
    Hash * h;
private slots:
    virtual void setup();
    virtual void constructor();
    void getStatus();
    void logIn();
    void logOff();
    void getName();
    void getIteration();
    void getSalt();
    void getID();
    void getLogin();
//    PackageWrapper *getPackageToSend();
//    bool hasPacketsToSend();
//    void pushPackageToSend(PackageWrapper *m);
//    void popPackageToSend();

    virtual void tearDown();
};
