/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <mutex>
#include <atomic>
#include <QtSql/QSqlDatabase>

#include "../../shared/id_client.h"
#include "../../shared/user.h"
#include "../../server/interfaces/interface_thread.h"


/**
\class
\brief
*/
class DataBase : public InterfaceThread
{
private:
    struct CallbackData
    {
        User* u;
    };

    std::map<ClientID, std::shared_ptr<User>> mUsers;
    std::string path;

    QSqlDatabase db;

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
    void createDatabase();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual void dowork();
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void countAttempsToLogin(MessagePing *ping);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void saltRequest(std::string login);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void RequestStart();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void RequestStop();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    DataBase(ThreadConfiguration &conf, std::string _path);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void loginRequest(std::string login, Hash *password);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void registerUserToDatabase(std::string name, ClientID id, std::string login, Hash *password, int iteration, uint32_t salt);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void ReleaseResources();
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
    void removeUser(std::shared_ptr<User> u);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void removeUser(ClientID id);

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void getUserViaID(ClientID id);
};

#endif // DATABASE_H
