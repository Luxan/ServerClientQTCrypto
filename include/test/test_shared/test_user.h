#pragma once

#include <QtTest/QtTest>

#include "../../../include/shared/user.h"
#include "../../../include/shared/id_client.h"
#include "../../../include/test/test_base.h"

class Test_User : public TestBase
{
    Q_OBJECT
private:
    UserCredentials * credentials;
    UserRelations * relations;
    size_t size;
    User * u;
    Hash * h;
private slots:
    virtual void setup()
    {
        size = rand() % 100;
        uint8_t tab[size];
        for (size_t i = 0; i < size; i++)
        {
            tab[i] = i;
        }
        h = new Hash(tab, sizeof(tab));
        credentials = new UserCredentials("somelogin", h, 5, 12345, ClientID(4));
        relations = new UserRelations();
    }

    virtual void constructor()
    {
        u = new User("somename", credentials, relations);
    }

    void getStatus()
    {
        QCOMPARE(u->getStatus(), Status::Offline);
    }

    void logIn()
    {
        QCOMPARE(u->getStatus(), Status::Offline);
        QVERIFY(u->logIn("somelogin", h));
        QCOMPARE(u->getStatus(), Status::Online);
    }

    void logOff()
    {
        QCOMPARE(u->getStatus(), Status::Online);
        u->logOff();
        QCOMPARE(u->getStatus(), Status::Offline);
    }

    void getName()
    {
        QCOMPARE(u->getName(), std::string("somename"));
    }

    void getIteration()
    {
        QCOMPARE(u->getIteration(), (uint64_t)5);
    }

    void getSalt()
    {
        QCOMPARE(u->getSalt(), (uint64_t)12345);
    }


    void getID()
    {
        QCOMPARE(u->getID(), ClientID(4));
    }

    void getLogin()
    {
        QCOMPARE(u->getLogin(), std::string("somelogin"));
    }

//    PackageWrapper *getPackageToSend();
//    bool hasPacketsToSend();
//    void pushPackageToSend(PackageWrapper *m);
//    void popPackageToSend();

    virtual void tearDown()
    {
        delete u;
    }
};
