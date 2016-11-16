#include "../../../include/test/test_shared/test_user.h"

void Test_User::setup()
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

void Test_User::constructor()
{
    u = new User("somename", credentials, relations);
}

void Test_User::getStatus()
{
    QCOMPARE(u->getStatus(), Status::Offline);
}

void Test_User::logIn()
{
    QCOMPARE(u->getStatus(), Status::Offline);
    QVERIFY(u->logIn("somelogin", h));
    QCOMPARE(u->getStatus(), Status::Online);
}

void Test_User::logOff()
{
    QCOMPARE(u->getStatus(), Status::Online);
    u->logOff();
    QCOMPARE(u->getStatus(), Status::Offline);
}

void Test_User::getName()
{
    QCOMPARE(u->getName(), std::string("somename"));
}

void Test_User::getIteration()
{
    QCOMPARE(u->getIteration(), (uint64_t)5);
}

void Test_User::getSalt()
{
    QCOMPARE(u->getSalt(), (uint64_t)12345);
}

void Test_User::getID()
{
    QCOMPARE(u->getID(), ClientID(4));
}

void Test_User::getLogin()
{
    QCOMPARE(u->getLogin(), std::string("somelogin"));
}

//    PackageWrapper *getPackageToSend();
//    bool hasPacketsToSend();
//    void pushPackageToSend(PackageWrapper *m);
//    void popPackageToSend();

void Test_User::tearDown()
{
    delete u;
}
