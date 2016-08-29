/**
\author Sergey Gorokh (ESEGORO)
*/
#include <string>
#include <mutex>

#include "../../include/shared/user.h"
#include "../../include/shared/id_client.h"
#include "../../include/shared/package.h"

User::User(uint32_t _id, std::string _login, std::string _password):
    login(_login), password(_password), id(_id)
{}

bool User::LogIn(std::string _login, std::string _password)
{
    std::lock_guard<std::mutex> guard(lock_data);
    if (login == _login)
    {
        if(password == _password)
        {
            status = Status::Online;
            return true;
        }
    }

    return false;
}

void User::LogOff()
{
    status = Status::Offline;
}

std::string User::getName()const
{
    return name;
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

std::string User::getPassword()const
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
