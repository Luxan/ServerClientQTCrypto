/**
\author Sergey Gorokh (ESEGORO)
*/
#include <thread>
#include <mutex>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include "../../include/server/slog.h"

#include "../../include/server/database.h"
#include "../../include/shared/user_credentials.h"
#include "../../include/shared/user_relations.h"

std::mutex DataBase::map_lock;
std::map<ClientID, std::shared_ptr<User>> DataBase::mUsers;
std::string DataBase::path;

void DataBase::CreateDatabase()
{
    QSqlQuery query;
    try
    {
        query.exec("CREATE TABLE USER("  \
                   "ID INT PRIMARY KEY        NOT NULL," \
                   "LOGIN          CHAR(50)   NOT NULL," \
                   "PASSWORD       CHAR(50)   NOT NULL);");
    }
    catch (std::string e)
    {
        SLog::logError() << "SQL error: Cant create table! " + e;
    }

    SLog::logInfo() << "Table created successfully.";
}

DataBase::DataBase()
{
    std::lock_guard<std::mutex> guard(map_lock);
    try
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QString(path.c_str()));
        db.open();
    }
    catch (std::string e)
    {
        throw ("SQL error: cant Open Database! " + e);
    }
    SLog::logInfo() << "Opened database successfully.";
}

bool DataBase::LoadResources(std::string _path)
{
    path = _path;
    DataBase *db;
    try
    {
        db = new DataBase();
        db->CreateDatabase();
        db->loadAllUsersFromDatabase();
    }
    catch (std::string e)
    {
        SLog::logError() << e;
        delete db;
        return false;
    }
    delete db;
    return true;
}

std::shared_ptr<User> DataBase::selectUserFromDatabase(ClientID id)
{
    try
    {
        /* Create SQL statement */
        QSqlQuery query(QString("SELECT * from USER where ID=") + ((std::string)id).c_str() + QString(";"));

        int fieldNameNo = query.record().indexOf("NAME");
        int fieldIDNo = query.record().indexOf("ID");
        int fieldLoginNo = query.record().indexOf("LOGIN");
        int fieldPasswordNo = query.record().indexOf("PASSWORD");
        int fieldIterationNo = query.record().indexOf("ITERATION");
        int fieldSaltNo = query.record().indexOf("SALT");
        int fieldFriendListNo = query.record().indexOf("FRIENDLIST");
        int fieldBlockedUserListNo = query.record().indexOf("BLOCKEDUSERLIST");
        int fieldPresenceInBlockedListNo = query.record().indexOf("PRESENCEINBLOCKEDLIST");
        int fieldPresenceInRoomsNo = query.record().indexOf("PRESENCEINROOMS");
        User *user;
        while (query.next())
        {
            std::string name = query.value(fieldNameNo).toString().toStdString();
            int id = query.value(fieldIDNo).toInt();
            std::string login = query.value(fieldLoginNo).toString().toStdString();
            QByteArray password = query.value(fieldPasswordNo).toByteArray();
            int iteration = query.value(fieldIterationNo).toInt();
            uint32_t salt = query.value(fieldSaltNo).toUInt();
            QString friendList = query.value(fieldFriendListNo).toString();
            QString blockedUserList = query.value(fieldBlockedUserListNo).toString();
            QString presenceInBlockedList = query.value(fieldPresenceInBlockedListNo).toString();
            QString presenceInRooms = query.value(fieldPresenceInRoomsNo).toString();

            Hash * hashedPassword = new Hash(password.data_ptr(), password.length());
            UserCredentials *uc = new UserCredentials(login, hashedPassword, iteration, salt, id);
            UserRelations * ur = new UserRelations(friendList, blockedUserList, presenceInBlockedList, presenceInRooms);
            user = new User(name, uc, ur);
            std::shared_ptr<User> u(user);

            SLog::logInfo() << "User selected successfully";
            return u;
        }
    }
    catch(std::string e)
    {
        throw("SQL error: cant select user from database! " + e);
    }
    return nullptr;
}

void DataBase::loadAllUsersFromDatabase()
{
    try
    {
        /* Create SQL statement */
        QSqlQuery query(QString("SELECT * from USER"));

        int fieldIDNo = query.record().indexOf("ID");
        int fieldLoginNo = query.record().indexOf("LOGIN");
        int fieldPasswordNo = query.record().indexOf("PASSWORD");
        User *user;
        while (query.next())
        {
            int ID = query.value(fieldIDNo).toInt();
            std::string Login = query.value(fieldLoginNo).toString().toStdString();
            std::string Password = query.value(fieldPasswordNo).toString().toStdString();

            user = new User(ID, Login, Password);
            std::shared_ptr<User> u(user);
            mUsers.insert(std::pair<ClientID, std::shared_ptr<User>>(u->getID(), u));
        }
    }
    catch (std::string e)
    {
        throw ("SQL error: cant load all users in Database! " + e);
    }
    SLog::logInfo() << "All user in database loaded successfully.";
}

void DataBase::deleteUserInDatabase(ClientID u)
{
    try
    {
        QSqlQuery query(QString("DELETE from USER where ID=") + ((std::string)u).c_str() + QString(";"));
    }
    catch (std::string e)
    {
        throw ("SQL error: cant delete user in Database! " + e);
    }
    SLog::logInfo() << "User in database deleted successfully.";
}

void DataBase::insertUserToDatabase(std::shared_ptr<User> u)
{
    QSqlQuery query;
    try
    {
        query.prepare("INSERT INTO USER (ID,LOGIN,PASSWORD) "
                      "VALUES (:id, :login, :password)");
        query.bindValue(":id", (int)u->getID());
        query.bindValue(":login", u->getLogin().c_str());
        query.bindValue(":password", u->getPassword().c_str());
        query.exec();
    }
    catch(std::string e)
    {
        throw("SQL error: Cant insert user into database! " + e);
    }

    SLog::logInfo() << "User inserted successfully.";
}

void DataBase::updateUserInDatabase(std::shared_ptr<User> u)
{
    QSqlQuery query;
    try
    {
        query.prepare("UPDATE USER (ID,LOGIN,PASSWORD) "
                      "VALUES (:id, :login, :password)");
        query.bindValue(":id", (int)u->getID());
        query.bindValue(":login", u->getLogin().c_str());
        query.bindValue(":password", u->getPassword().c_str());
        query.exec();
    }
    catch(std::string e)
    {
        throw("SQL error: Cant update user into database! " + e);
    }

    SLog::logInfo() << "User updated successfully.";
}

void DataBase::ReleaseResources()
{
    std::lock_guard<std::mutex> guard(map_lock);
    mUsers.clear();
}

DataBase &DataBase::GetDataBase()
{
    static DataBase s;
    return s;
}

DataBase::~DataBase()
{
    db.close();
}

bool DataBase::addUser(std::shared_ptr<User> u)
{
    std::lock_guard<std::mutex> guard(map_lock);

    auto it = mUsers.find(u->getID());
    if (it == mUsers.end())
    {
        mUsers.insert(std::pair<ClientID, std::shared_ptr<User>>(u->getID(), u));

        insertUserToDatabase(u);

        return true;
    }

    return false;
}

bool DataBase::removeUser(std::shared_ptr<User> u)
{
    std::lock_guard<std::mutex> guard(map_lock);

    auto it = mUsers.find(u->getID());
    if (it != mUsers.end())
    {
        mUsers.erase(u->getID());

        deleteUserInDatabase(u->getID());
        return true;
    }

    return false;
}

std::shared_ptr<User> DataBase::getUser(ClientID id)
{
    std::lock_guard<std::mutex> guard(map_lock);

    auto it = mUsers.find(id);
    if (it != mUsers.end())
    {
        return (*it).second;
    }

    return nullptr;
}

std::shared_ptr<User> DataBase::getUser(std::string &login, std::string &password)
{
    for (auto it = mUsers.begin(); it != mUsers.end(); it++)
    {
        std::shared_ptr<User> u = (*it).second;
        if (u->LogIn(login, password))
        {
            return u;
        }
    }
    return nullptr;
}

