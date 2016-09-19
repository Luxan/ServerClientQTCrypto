#pragma once

#include <QString>
#include <QStringList>
#include <stdint.h>
#include <list>
#include "id_client.h"
#include "id_room.h"
/*!
\class UserRelations
\brief class that contains User's relations to other Users.
*/
class UserRelations
{
    std::list<ClientID> friend_list; /*! User's friends list */
    std::list<ClientID> blocked_user_list; /*! User's blocked list. Make impossible to receive messages from Users in this list. */
    std::list<ClientID> presence_in_blocked_list; /*! User's presense in blocked list. Make impossible to send to Users in this list*/
    std::list<RoomID> presence_in_rooms; /*! User's presense in room list. List of rooms in which User was participated. */
public:
    /*!
    \brief constructor to initialize UserRelations class
    \param friendList - list of friends
    \param blockedUserList - list of blocked users
    \param presenseInBlockedList - list of users which blocked this user
    \param presenseInRooms - list of rooms
    */
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
    /*! 
    \brief default contructor that creates empty lists.
    */
    UserRelations()
    {}
};
