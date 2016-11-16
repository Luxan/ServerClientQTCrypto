/**
\author Sergey Gorokh (ESEGORO)
*/
#include <thread>
#include <mutex>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QVariant>

#include "../../../include/server/login_server/server_logger.h"

#include "../../../../include/server/modules/database.h"
#include "../../../include/shared/user_credentials.h"
#include "../../../include/shared/user_relations.h"

void DataBase::createDatabase()
{
    QSqlQuery query;
    try
    {
        query.exec("CREATE TABLE USER("  \
                   "NAME                    CHAR(50)            NOT NULL,"
                   "ID                      INT PRIMARY KEY     NOT NULL," \
                   "LOGIN                   CHAR(50)            NOT NULL," \
                   "PASSWORD                BLOB                NOT NULL,"\
                   "ITERATION               INT                 NOT NULL,"\
                   "SALT                    INT                 NOT NULL,"\
                   "FRIENDLIST              CLOB                ,"\
                   "BLOCKEDUSERLIST         CLOB                ,"\
                   "PRESENCEINBLOCKEDLIST   CLOB                ,"\
                   "PRESENCEINROOMS         CLOB                ,"\
                   ");");
    }
    catch (std::string e)
    {
        LOG_ERROR("SQL error: Cant create table! " + e);
    }

    LOG_INFO("Table created successfully.");
}

void DataBase::dowork()
{

}
void DataBase::RequestStart()
{
    if (isRunning())
    {
        std::string error = "Database is already running.";
        AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorDatabase, error));
        return;
    }

    unSleepThread();
}

void DataBase::RequestStop()
{
    if (!isRunning())
    {
        std::string error = "Database is already stopped.";
        AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorDatabase, error));
        return;
    }

    sleepThread();
}

DataBase::DataBase(ThreadConfiguration& conf, std::string _path):
    InterfaceThread(conf)
{
    path = _path;
    try
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QString(path.c_str()));
        db.open();
        createDatabase();
        loadAllUsersFromDatabase();
    }
    catch (std::string e)
    {
        throw ("SQL error: Start up Database! " + e);
    }
    LOG_INFO("Opened database successfully.");
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
            uint32_t id = query.value(fieldIDNo).toUInt();
            std::string login = query.value(fieldLoginNo).toString().toStdString();
            QByteArray password = query.value(fieldPasswordNo).toByteArray();
            uint32_t iteration = query.value(fieldIterationNo).toUInt();
            uint64_t salt = query.value(fieldSaltNo).toULongLong();
            QString friendList = query.value(fieldFriendListNo).toString();
            QString blockedUserList = query.value(fieldBlockedUserListNo).toString();
            QString presenceInBlockedList = query.value(fieldPresenceInBlockedListNo).toString();
            QString presenceInRooms = query.value(fieldPresenceInRoomsNo).toString();

            Hash * hashedPassword = new Hash((uint8_t*)password.constData(), password.length());
            UserCredentials *uc = new UserCredentials(login, hashedPassword, iteration, salt, id);
            UserRelations * ur = new UserRelations(friendList, blockedUserList, presenceInBlockedList, presenceInRooms);
            user = new User(name, uc, ur);
            std::shared_ptr<User> u(user);

            LOG_INFO("User selected successfully");
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
            uint32_t id = query.value(fieldIDNo).toUInt();
            std::string login = query.value(fieldLoginNo).toString().toStdString();
            QByteArray password = query.value(fieldPasswordNo).toByteArray();
            uint32_t iteration = query.value(fieldIterationNo).toUInt();
            uint64_t salt = query.value(fieldSaltNo).toULongLong();
            QString friendList = query.value(fieldFriendListNo).toString();
            QString blockedUserList = query.value(fieldBlockedUserListNo).toString();
            QString presenceInBlockedList = query.value(fieldPresenceInBlockedListNo).toString();
            QString presenceInRooms = query.value(fieldPresenceInRoomsNo).toString();

            Hash * hashedPassword = new Hash((uint8_t*)password.constData(), password.length());
            UserCredentials *uc = new UserCredentials(login, hashedPassword, iteration, salt, id);
            UserRelations * ur = new UserRelations(friendList, blockedUserList, presenceInBlockedList, presenceInRooms);
            user = new User(name, uc, ur);
            std::shared_ptr<User> u(user);

            mUsers.insert(std::pair<ClientID, std::shared_ptr<User>>(u->getID(), u));

            LOG_INFO("User selected successfully");
        }
    }
    catch (std::string e)
    {
        throw ("SQL error: cant load all users in Database! " + e);
    }
    LOG_INFO("All user in database loaded successfully.");
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
    LOG_INFO("User in database deleted successfully.");
}

void DataBase::registerUserToDatabase(std::string name, ClientID id, std::string login, Hash * password, int iteration, uint32_t salt)
{
    UserCredentials *uc = new UserCredentials(login, password, iteration, salt, id);
    UserRelations * ur = new UserRelations();
    User * user = new User(name, uc, ur);
    std::shared_ptr<User> u(user);

    QSqlQuery query;
    try
    {
        query.prepare("INSERT INTO USER (NAME,ID,LOGIN,PASSWORD,ITERATION,SALT) "
                      "VALUES (:name, :id, :login, :password, "\
                      ":iteration, :salt)");
        query.bindValue(":name", name.c_str());
        query.bindValue(":id", id.operator uint32_t());
        query.bindValue(":login", login.c_str());
        query.bindValue(":password", QByteArray((const char *)password->getPointerToBuffer(), password->getLength()));
        query.bindValue(":iteration", iteration);
        query.bindValue(":salt", salt);
        query.exec();

        mUsers.insert(std::pair<ClientID, std::shared_ptr<User>>(u->getID(), u));
    }
    catch(std::string e)
    {
        throw("SQL error: Cant insert user into database! " + e);
    }

    LOG_INFO("User inserted successfully.");
}

void DataBase::updateUserInDatabase(std::shared_ptr<User> u)
{
    QSqlQuery query;
//    try
//    {
//        query.prepare("INSERT INTO USER (NAME,FRIENDLIST,BLOCKEDUSERLIST,PRESENCEINBLOCKEDLIST,PRESENCEINROOMS) "
//                      "VALUES (:name, :friendlist, "\
//                      ":blockuserlist, :presenceblockedlist, "\
//                      ":presenceinrooms)");
//        query.bindValue(":name", u->getName().c_str());
//        query.bindValue(":friendlist", u->getPassword().c_str());
//        query.bindValue(":blockuserlist", u->getPassword().c_str());
//        query.bindValue(":presenceblockedlist", u->getPassword().c_str());
//        query.bindValue(":presenceinrooms", u->getPassword().c_str());
//        query.exec();
//    }
//    catch(std::string e)
//    {
//        throw("SQL error: Cant update user into database! " + e);
//    }

    LOG_INFO("User updated successfully.");
}

void DataBase::ReleaseResources()
{
    mUsers.clear();
}

DataBase::~DataBase()
{
    db.close();
    ReleaseResources();
}

void DataBase::removeUser(std::shared_ptr<User> u)
{
    removeUser(u->getID());
}

void DataBase::removeUser(ClientID id)
{
    auto it = mUsers.find(id);
    if (it != mUsers.end())
    {
        mUsers.erase(id);

        deleteUserInDatabase(id);
    }

    this->AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorCannotRemoveUserFromDatabase, "User with id: " + std::to_string(id) + "is not exist in database!"));
}

void DataBase::getUserViaID(ClientID id)
{
    auto it = mUsers.find(id);
    if (it != mUsers.end())
    {
        this->AddImpulseToQueue(new ImpulseUser(eSystemEvent::DatabaseGetUserByID, (*it).second));
    }

    this->AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorCannotFindUserFromDatabase, "User with id: " + std::to_string(id) + "is not exist in database!"));
}

void DataBase::countAttempsToLogin(MessagePing *ping)
{
    for (auto it = mUsers.begin(); it != mUsers.end(); it++)
    {
        std::shared_ptr<User> u = (*it).second;
        if (u->getLogin() == ping->getLogin())
        {
            //if ()
            //this->AddImpulseToQueue(new ImpulseSalt(eSystemEvent::DatabaseUserSalt, u->getSalt()));
            delete ping;
            return;
        }
    }
    delete ping;
    //this->AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorCannotFindUserFromDatabase, "User with login: " + login + "is not exist in database!"));
}

void DataBase::saltRequest(std::string login)
{
    for (auto it = mUsers.begin(); it != mUsers.end(); it++)
    {
        std::shared_ptr<User> u = (*it).second;
        if (u->getLogin() == login)
        {
            this->AddImpulseToQueue(new ImpulseSalt(eSystemEvent::DatabaseUserSalt, u->getSalt()));
            return;
        }
    }
    this->AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorCannotFindUserFromDatabase, "User with login: " + login + "is not exist in database!"));
}

void DataBase::loginRequest(std::string login, Hash * password)
{
    for (auto it = mUsers.begin(); it != mUsers.end(); it++)
    {
        std::shared_ptr<User> u = (*it).second;
        if (u->logIn(login, password))
        {
            this->AddImpulseToQueue(new ImpulseUser(eSystemEvent::DatabaseLoginSuccessfull, (*it).second));
        }
    }
    this->AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorCannotFindUserFromDatabase, "User with login: " + login + "is not exist in database!"));
}

