#pragma once
#include "../shared/id_client.h"

class RoomID : public ClientID
{
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    RoomID(const RoomID &c):
        ClientID(c)
    {}

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    RoomID(uint32_t _id):
        ClientID(_id)
    {}

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    RoomID():
        ClientID()
    {}
};

