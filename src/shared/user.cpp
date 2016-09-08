/**
\author Sergey Gorokh (ESEGORO)
*/
#include <string>
#include <mutex>

#include "../../include/shared/user.h"
#include "../../include/shared/id_client.h"
#include "../../include/shared/package.h"
#include "../../include/shared/hash.h"

User::User(std::string name, UserCredentials * credentials, UserRelations * relations):
    name(name), credentials(credentials), relations(relations)
{}

bool User::LogIn(std::string _login, Hash * _password)
{
    std::lock_guard<std::mutex> guard(lock_data);
    if (credentials->getLogin() == _login)
    {
        if(credentials->isCorrectPassword(_password))
        {
            status = Status::Online;
            return true;
        }
    }

    return false;
}

User::~User()
{
    delete credentials;
    delete relations;
}

void User::LogOff()
{
    status = Status::Offline;
}

std::string User::getName()
{
    return name;
}

uint64_t User::getIteration()
{
    return credentials->getIteration();
}
uint64_t User::getSalt()
{
    return credentials->getSalt();
}

Status User::getStatus()
{
    return status;
}

ClientID User::getID()
{
    return credentials->getID();
}

std::string User::getLogin()
{
    return credentials->getLogin();
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
