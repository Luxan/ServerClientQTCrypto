/**
\author Sergey Gorokh (ESEGORO)
*/
#include <string>
#include <iostream>
#include "../../include/shared/id_client.h"

ClientID::ClientID(uint32_t _id):
    id(_id)
{
    if(id > staticID) staticID = id;
}

ClientID::ClientID(const ClientID &c)
{
    id = c.id;
    if(id > staticID) staticID = id;
}

ClientID::ClientID()
{
    id = ++staticID;
}
ClientID::ClientID(std::string &data)
{
    this->id = std::stoi(data);
    if(id > staticID) staticID = id;
}

ClientID::ClientID(const char *data)
{
    this->id = std::stoi(data);
    if(id > staticID) staticID = id;
}

bool ClientID::operator ==(const ClientID& id)const
{
    return this->id == id.id;
}

bool ClientID::operator ==(const int id)const
{
    return this->id == id;
}

bool ClientID::operator<(const ClientID &id)const
{
    return this->id < id.id;
}

ClientID::operator std::string() const
{
    return std::to_string(id);
}

ClientID::operator uint32_t() const
{
    return id;
}

bool ClientID::operator>(const ClientID &id)const
{
    return this->id > id.id;
}
ClientID ClientID::operator=(uint32_t _id)
{
    id = _id;
    if(id > staticID) staticID = id;
    return *this;
}

int ClientID::staticID = 0;
