/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <mutex>
#include <atomic>
#include <QSqlDatabase>

#include "../shared/id_client.h"
#include "../shared/user.h"


/**
\class
\brief
*/
class DataBase
{
private:
    struct CallbackData
    {
        User* u;
    };

    static std::mutex map_lock;
    static std::map<ClientID, std::shared_ptr<User>> mUsers;

    static std::string path;

    QSqlDatabase db;

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    DataBase();

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    void deleteUserInDatabase(ClientID u);
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    void updateUserInDatabase(std::shared_ptr<User> u);
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    std::shared_ptr<User> selectUserFromDatabase(ClientID id);
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    void loadAllUsersFromDatabase();
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    void insertUserToDatabase(std::shared_ptr<User> u);

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    void CreateDatabase();
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    std::shared_ptr<User> getUser(std::string &login, std::string &password);

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    static bool LoadResources(std::string _path);

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    static void ReleaseResources();

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    static DataBase &GetDataBase();

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ~DataBase();

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool addUser(std::shared_ptr<User> u);

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool removeUser(std::shared_ptr<User> u);

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    std::shared_ptr<User> getUser(ClientID id);
};

#endif // DATABASE_H
