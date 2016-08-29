/**
\author Sergey Gorokh (ESEGORO)
*/
#include <string>
#include <iostream>
#include "../../include/shared/id_client.h"

ClientID::ClientID(uint32_t &_id):
    id(_id)
{}

ClientID::ClientID(const ClientID &c)
{
    id = c.id;
}

ClientID::ClientID()
{
    staticID++;
    id = staticID;
}
ClientID::ClientID(std::string &data)
{
    this->id = std::stoi(data);
}

bool ClientID::operator ==(const ClientID& id)const
{
    return this->id == id.id;
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

int ClientID::staticID = 0;
