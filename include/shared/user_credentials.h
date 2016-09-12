#pragma once

#include <mutex>
#include "crypto/hash.h"
#include "id_client.h"

class UserCredentials
{
private:
    std::mutex credentials_lock;
    std::string login;
    Hash * password;
    uint32_t password_hash_iteration_count;
    uint64_t user_salt;
    ClientID id;
public:
    std::string getLogin()
    {
        std::lock_guard<std::mutex> guard(credentials_lock);
        return login;
    }
    Hash * getPassword()
    {
        std::lock_guard<std::mutex> guard(credentials_lock);
        return password;
    }

    uint64_t getSalt()
    {
        std::lock_guard<std::mutex> guard(credentials_lock);
        return user_salt;
    }

    ClientID getID()
    {
        std::lock_guard<std::mutex> guard(credentials_lock);
        return id;
    }

    uint32_t getIteration()
    {
        std::lock_guard<std::mutex> guard(credentials_lock);
        return password_hash_iteration_count;
    }

    bool isCorrectPassword(Hash * passwordToCheck)
    {
        std::lock_guard<std::mutex> guard(credentials_lock);
        return password->operator ==(*passwordToCheck);
    }

    UserCredentials(std::string login,
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
