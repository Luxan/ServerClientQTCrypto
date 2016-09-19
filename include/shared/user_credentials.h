#pragma once

#include <mutex>
#include "crypto/hash.h"
#include "id_client.h"
/*!
\class UserCredentials
\brief contains user's credentials such as password, id or login etc.
\thread is thread safe. Has mutex to data stored in class.
*/
class UserCredentials
{
private:
    std::mutex credentials_lock; /*! lock to stored data to protect this data from access form 2 different threads. */
    std::string login; /*! user's login */
    Hash * password; /*! user's hashed password with salt */
    uint32_t password_hash_iteration_count; /*! user's iteration count of hashing function that hashes user's password with salt */
    uint64_t user_salt; /*! user's salt */
    ClientID id; /*! user's unique id */
public:
    /*!
    \return user's login
    \brief returns std::string login
    */
    std::string getLogin()
    {
        std::lock_guard<std::mutex> guard(credentials_lock);
        return login;
    }
    /*!
    \return user's hashed password with salt
    \brief returns user's hashed password with salt
    */
    Hash * getPassword()
    {
        std::lock_guard<std::mutex> guard(credentials_lock);
        return password;
    }

    /*!
    \return user's salt
    \brief returns user's salt
    */
    uint64_t getSalt()
    {
        std::lock_guard<std::mutex> guard(credentials_lock);
        return user_salt;
    }
    /*!
    \return user's unique id
    \brief returns user's unique id
    */
    ClientID getID()
    {
        std::lock_guard<std::mutex> guard(credentials_lock);
        return id;
    }
    /*!
    \return user's iteration count of hashing function that hashes user's password with salt
    \brief returns user's iteration count of hashing function that hashes user's password with salt
    */
    uint32_t getIteration()
    {
        std::lock_guard<std::mutex> guard(credentials_lock);
        return password_hash_iteration_count;
    }

    /*!
    \return true if they are the same
    \param passwordToCheck user's hashed password with salt to check
    \brief 
    */
    bool isCorrectPassword(Hash * passwordToCheck)
    {
        std::lock_guard<std::mutex> guard(credentials_lock);
        return password->operator ==(*passwordToCheck);
    }

    UserCredentials(std::string &login,
                    Hash * password,
                    uint32_t password_hash_iteration_count,
                    uint64_t user_salt,
                    ClientID id):
        login(login),
        password(password),
        password_hash_iteration_count(password_hash_iteration_count),
        user_salt(user_salt),
        id(id)
    {}

    ~UserCredentials()
    {
        delete password;
    }
};
