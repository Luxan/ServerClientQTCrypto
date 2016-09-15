/*!
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <stdint.h>
#include <string>

/*!
\class ClientID
\brief class that is used to identify client. It contains original number for every user. 
*/
class ClientID
{
private:
    uint32_t id; /*! Original number for every user */
    static int staticID; /*! Number for next original user that will be created. It will be incremented due creation of next User. */
public:
    /*!
    \brief operator == that checks if given object of ClientID is equal to present.
    \param id - object of ClientID that will be compared to present object.
    \return true if both objects has same original id.
    */
    bool operator==(const ClientID &id)const;
    /*!
    \brief operator < that checks if given object of ClientID is smaller to present.
    \param id - object of ClientID that will be compared to present object.
    \return true if given object has greater original id that present object.
    */
    bool operator<(const ClientID &id)const;
    /*!
    \brief cast operator to std::string
    */
    operator std::string() const;    
    /*!
    \brief cast operator to uint32_t
    */
    operator uint32_t() const;
    /*!
    \brief operator > that checks if given object of ClientID is greater to present.
    \param id - object of ClientID that will be compared to present object.
    \return true if given object has smaller original id that present object.
    */
    bool operator>(const ClientID &id)const;
    /*!
    \brief cast operator from uint32_t to ClientID
    \param _id - original id of client
    \return object of ClientID with given original id
    */
    ClientID operator=(uint32_t _id);
    /*!
    \brief copy constructor to copy original id of given object to new object
    \param c - reference of object which id willl be copyed to new object 
    */
    ClientID(const ClientID &c);
    /*!
    \brief contructor to initialize original id with given number
    \param _id - reference of number to initialize original id
    */
    ClientID(uint32_t &_id);
    /*!
    \brief default contructor that initialize original id with static number staticID
    */
    ClientID();
    /*!
    \brief contructor to retrieve original id of User from given string
    \param data - reference to std::string that will initialize original id of User
    */
    ClientID(std::string &data);
};

