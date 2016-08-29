#pragma once

#include "../shared/id_client.h"

class MessageID : public ClientID
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
    MessageID(const MessageID &c):
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
    MessageID(uint32_t _id):
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
    MessageID():
        ClientID()
    {}
};
