/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <stdint.h>
#include <string>

/**
\class
\brief
*/
class ClientID
{
private:
    uint32_t id;
    static int staticID;
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool operator==(const ClientID &id)const;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool operator<(const ClientID &id)const;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    operator std::string() const;    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    operator uint32_t() const;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool operator>(const ClientID &id)const;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ClientID operator=(uint32_t _id);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ClientID(const ClientID &c);

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ClientID(uint32_t &_id);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ClientID();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ClientID(std::string &data);
};

