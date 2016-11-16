#include "../../../include/test/test_shared/test_user_relations.h"

void Test_UserRelations::setup()
{

}

void Test_UserRelations::constructor()
{
    relations = new UserRelations(friendList, blockedUserList, presenseInBlockedList, presenseInRooms);
}

void Test_UserRelations::tearDown()
{
    delete relations;
}
