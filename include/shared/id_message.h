#pragma once

#include "../shared/id_client.h"
/*!
\class MessageID
\brief class that is used to identify message. It contains original number for every message. 
*/
class MessageID : public ClientID
{
public:
    /*!
    \brief copy constructor to copy original id of given object to new object
    \param c - reference of object which id will be copyed to new object 
    */
    MessageID(const MessageID &c):
        ClientID(c)
    {}

    /*!
    \brief cast operator from uint32_t to MessageID
    \param _id - original id of message
    \return object of MessageID with given original id
    */
    MessageID(uint32_t _id):
        ClientID(_id)
    {}

    /*!
    \brief default contructor that initialize original id with static number staticID
    */
    MessageID():
        ClientID()
    {}
    /*!
    \brief contructor to retrieve original id of message from given const char *
    \param data - const char * that will initialize original id of message
    */
    MessageID(const char *data):
        ClientID(data)
    {}
};
