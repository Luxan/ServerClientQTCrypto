/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <string>
#include <mutex>
#include <queue>
#include <stdint.h>
#include <list>

#include "../shared/id_client.h"
#include "../shared/id_room.h"
#include "../shared/package.h"
#include "../shared/hash.h"
#include "../shared/status.h"
#include "../shared/user_credentials.h"
#include "../shared/user_relations.h"

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

    UserCredentials * credentials;
    UserRelations * relations;
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
    User(std::string name, UserCredentials * credentials, UserRelations * relations);
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
    std::string getName();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    uint64_t getIteration();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    uint64_t getSalt();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    Status getStatus();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ClientID getID();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    std::string getLogin();
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
