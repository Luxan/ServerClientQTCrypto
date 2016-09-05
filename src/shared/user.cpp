/**
\author Sergey Gorokh (ESEGORO)
*/
#include <string>
#include <mutex>

#include "../../include/shared/user.h"
#include "../../include/shared/id_client.h"
#include "../../include/shared/package.h"
#include "../../include/shared/hash.h"

User::User(uint32_t _id, std::string _login, uint8_t * _password, size_t passwordLength, uint64_t _user_salt):
    login(_login),
    user_salt(_user_salt),
    id(_id)
{
    password = new Hash(_password, passwordLength);
}

bool User::LogIn(std::string _login, Hash * _password)
{
    std::lock_guard<std::mutex> guard(lock_data);
    if (login == _login)
    {
        if(password->operator ==(*_password))
        {
            status = Status::Online;
            return true;
        }
    }

    return false;
}

User::~User()
{
    delete password;
}

void User::LogOff()
{
    status = Status::Offline;
}

std::string User::getName()const
{
    return name;
}

uint64_t User::getSalt()const
{
    return user_salt;
}

const Status &User::getStatus()const
{
    return status;
}

const ClientID &User::getID()const
{
    return id;
}

std::string User::getLogin()const
{
    return login;
}

Hash* User::getPassword()const
{
    return password;
}

PackageWrapper * User::getPackageToSend()
{
    std::lock_guard<std::mutex> guard(lock_queue);
    return vpackagesToSend.front();
}

void User::pushPackageToSend(PackageWrapper * m)
{
    std::lock_guard<std::mutex> guard(lock_queue);
    vpackagesToSend.push(m);
}

void User::popPackageToSend()
{
    std::lock_guard<std::mutex> guard(lock_queue);
    vpackagesToSend.pop();
}

bool User::hasPacketsToSend()
{
    std::lock_guard<std::mutex> guard(lock_queue);
    return !vpackagesToSend.empty();
}
