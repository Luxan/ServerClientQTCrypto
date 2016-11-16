#pragma once

#include <QtTest/QtTest>

#include "../../../include/shared/user_relations.h"
#include "../../../include/shared/id_client.h"
#include "../../../include/test/test_base.h"

class Test_UserRelations : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_UserRelations, TestBase)
private:
    UserRelations * relations;
    QString friendList;
    QString blockedUserList;
    QString presenseInBlockedList;
    QString presenseInRooms;
private slots:
    virtual void setup();
    virtual void constructor();
    virtual void tearDown();
};
