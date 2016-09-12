/**
\author Sergey Gorokh (ESEGORO)
*/
#include <thread>
#include <mutex>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include "../../include/server/slog.h"

#include "../../../include/server/modules/database.h"
#include "../../include/shared/user_credentials.h"
#include "../../include/shared/user_relations.h"

void DataBase::CreateDatabase()
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
        SLog::logError() << "SQL error: Cant create table! " + e;
    }

    SLog::logInfo() << "Table created successfully.";
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

void DataBase::dowork()
{

}

DataBase::DataBase(ThreadConfiguration &conf, std::string _path):
    interfaceThread(conf)
{
    path = _path;
    try
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QString(path.c_str()));
        db.open();
        db->CreateDatabase();
        loadAllUsersFromDatabase();
    }
    catch (std::string e)
    {
        throw ("SQL error: Start up Database! " + e);
    }
    SLog::logInfo() << "Opened database successfully.";
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

            mUsers.insert(std::pair<ClientID, std::shared_ptr<User>>(u->getID(), u));

            SLog::logInfo() << "User selected successfully";
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
        query.bindValue(":name", name);
        query.bindValue(":id", id.operator uint32_t);
        query.bindValue(":login", login);
        query.bindValue(":password", QByteArray(password->getBuff(), password->getLength()));
        query.bindValue(":iteration", iteration);
        query.bindValue(":salt", salt);
        query.exec();

        mUsers.insert(std::pair<ClientID, std::shared_ptr<User>>(u->getID(), u));
    }
    catch(std::string e)
    {
        throw("SQL error: Cant insert user into database! " + e);
    }

    SLog::logInfo() << "User inserted successfully.";
}

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

void DataBase::updateUserInDatabase(std::shared_ptr<User> u)
{
    QSqlQuery query;
    try
    {
        query.prepare("INSERT INTO USER (NAME,FRIENDLIST,BLOCKEDUSERLIST,PRESENCEINBLOCKEDLIST,PRESENCEINROOMS) "
                      "VALUES (:name, :friendlist, "\
                      ":blockuserlist, :presenceblockedlist, "\
                      ":presenceinrooms)");
        query.bindValue(":name", u->getName());
        query.bindValue(":friendlist", u->getPassword().c_str());
        query.bindValue(":blockuserlist", u->getPassword().c_str());
        query.bindValue(":presenceblockedlist", u->getPassword().c_str());
        query.bindValue(":presenceinrooms", u->getPassword().c_str());
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

DataBase::~DataBase()
{
    db.close();
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
        this->AddImpulseToQueue(new ImpulseUser(eSystemEvent::DatabaseGetUser, (*it).second));
    }

    this->AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorCannotFindUserFromDatabase, "User with id: " + std::to_string(u->getID()) + "is not exist in database!"));
}

void DataBase::saltRequest(std::string &login)
{
    for (auto it = mUsers.begin(); it != mUsers.end(); it++)
    {
        std::shared_ptr<User> u = (*it).second;
        if (u->getLogin() == login)
        {
            this->AddImpulseToQueue(new ImpulseUser(eSystemEvent::DatabaseUserSalt, u->getSalt()));
        }
    }
}

void DataBase::loginRequest(std::string &login, Hash * &password)
{
    for (auto it = mUsers.begin(); it != mUsers.end(); it++)
    {
        std::shared_ptr<User> u = (*it).second;
        if (u->LogIn(login, password))
        {
            this->AddImpulseToQueue(new ImpulseUser(eSystemEvent::DatabaseLoginSuccessfull, (*it).second));
        }
    }
    this->AddImpulseToQueue(new ImpulseError(eSystemEvent::ErrorCannotFindUserFromDatabase, "User with id: " + std::to_string(u->getID()) + "is not exist in database!"));
}

