#include "../../../include/test/test_shared/test_user_credentials.h"

void Test_UserCredentials::setup()
{
    uint8_t tab[10] = {0, 1,2,3,4,5,6,7,8,9};
    hash = new Hash(tab, sizeof(tab));
    login = "jacek";
    passwordItCount = 10;
    salt = 1234567890;
    id = 10;
}

void Test_UserCredentials::constructor()
{
    credentials = new UserCredentials(login, hash, passwordItCount, salt, id);

    QCOMPARE(credentials->getLogin(), login);
    QCOMPARE(credentials->getPassword(), hash);
    QCOMPARE(credentials->getIteration(), passwordItCount);
    QCOMPARE(credentials->getSalt(), salt);
    QCOMPARE(credentials->getID(), id);
}

void Test_UserCredentials::isCorrectPassword()
{
    QVERIFY(credentials->isCorrectPassword(hash));
}

void Test_UserCredentials::tearDown()
{
    delete credentials;
}
