#pragma once

#include "../shared/id_client.h"
/*!
\class SessionID
\brief class that is used to identify session. It contains original number for every session.
*/
class SessionID : public ClientID
{
public:
    /*!
    \brief copy constructor to copy original id of given object to new object
    \param c - reference of object which id will be copyed to new object 
    */
    SessionID(const SessionID &c):
        ClientID(c)
    {}

    /*!
    \brief cast operator from uint32_t to SessionID
    \param _id - original id of session
    \return object of SessionID with given original id
    */
    SessionID(uint32_t _id):
        ClientID(_id)
    {}

    /*!
    \brief default contructor that initialize original id with static number staticID
    */
    SessionID():
        ClientID()
    {}
    /*!
    \brief contructor to retrieve original id of session from given const char *
    \param data - const char * that will initialize original id of session
    */
    SessionID(const char *data):
        ClientID(data)
    {}
};
