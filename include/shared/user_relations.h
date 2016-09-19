#pragma once

#include <QString>
#include <QStringList>
#include <stdint.h>
#include <list>
#include "id_client.h"
#include "id_room.h"

class UserRelations
{
    std::list<ClientID> friend_list;
    std::list<ClientID> blocked_user_list;
    std::list<ClientID> presence_in_blocked_list;
    std::list<RoomID> presence_in_rooms;
public:
    UserRelations(QString friendList,
                  QString blockedUserList,
                  QString presenseInBlockedList,
                  QString presenseInRooms)
    {
        for (QString str : friendList.split(" "))
        {
            uint32_t s = str.toUInt();
            this->friend_list.push_back(ClientID(s));
        }
        for (QString str : blockedUserList.split(" "))
        {
            uint32_t s = str.toUInt();
            this->blocked_user_list.push_back(ClientID(s));
        }
        for (QString str : presenseInBlockedList.split(" "))
        {
            uint32_t s = str.toUInt();
            this->presence_in_blocked_list.push_back(ClientID(s));
        }
        for (QString str : presenseInRooms.split(" "))
        {
            uint32_t s = str.toUInt();
            this->presence_in_rooms.push_back(RoomID(s));
        }
    }

    UserRelations()
    {}
};
