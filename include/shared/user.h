/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <string>
#include <mutex>
#include <queue>
#include <stdint.h>

#include "../shared/id_client.h"
#include "../shared/package.h"
#include "../shared/hash.h"
#include "../shared/status.h"

/**
\class
\brief
*/
class User
{
private:
    std::mutex lock_data;
    std::mutex lock_queue;
    std::string name;
    std::string login;
    Hash * password;
    uint64_t user_salt;
    ClientID id;
    Status status;

    std::queue<PackageWrapper*> vpackagesToSend;
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    User(uint32_t _id, std::string _login, uint8_t * _password, size_t passwordLength, uint64_t _user_salt);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ~User();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool LogIn(std::string _login, Hash * _password);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void LogOff();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    std::string getName()const;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    uint64_t getSalt()const;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    const Status &getStatus()const;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    const ClientID &getID()const;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    std::string getLogin()const;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    Hash *getPassword()const;

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageWrapper *getPackageToSend();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool hasPacketsToSend();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void pushPackageToSend(PackageWrapper *m);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void popPackageToSend();
};
