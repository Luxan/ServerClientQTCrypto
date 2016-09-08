#pragma once

#include <QString>
#include <QStringList>
#include <list>
#include "id_client.h"
#include "id_room.h"

class UserRelations
{
    std::list<ClientID> friend_list;
    std::list<ClientID> blocked_user_list;
    std::list<ClientID> presence_in_blocked_list;
    std::list<RoomID> presence_in_rooms;

    UserRelations(QString friendList,
                  QString blockedUserList,
                  QString presenseInBlockedList,
                  QString presenseInRooms)
    {
        for (QString str : friendList.split(" "))
        {
            this->friend_list.push_back((ClientID)str.toUInt());
        }
        for (QString str : blockedUserList.split(" "))
        {
            this->blocked_user_list.push_back(ClientID(str.toUInt()));
        }
        for (QString str : presenseInBlockedList.split(" "))
        {
            this->presence_in_blocked_list.push_back(ClientID(str.toUInt()));
        }
        for (QString str : presenseInRooms.split(" "))
        {
            this->presence_in_rooms.push_back(RoomID(str.toUInt()));
        }
    }
};
