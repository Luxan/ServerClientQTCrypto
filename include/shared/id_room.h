#pragma once
#include "../shared/id_client.h"
/*!
\class RoomID
\brief class that is used to identify message. It contains original number for every room. 
*/
class RoomID : public ClientID
{
public:
    /*!
    \brief copy constructor to copy original id of given object to new object
    \param c - reference of object which id will be copyed to new object 
    */
    RoomID(const RoomID &c):
        ClientID(c)
    {}

    /*!
    \brief cast operator from uint32_t to RoomID
    \param _id - original id of room
    \return object of RoomID with given original id
    */
    RoomID(uint32_t _id):
        ClientID(_id)
    {}

    /*!
    \brief default contructor that initialize original id with static number staticID
    */
    RoomID():
        ClientID()
    {}
};

