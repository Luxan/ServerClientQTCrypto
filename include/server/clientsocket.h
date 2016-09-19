#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <memory>
#include "../shared/user.h"
#include "../shared/package_buffer.h"

class RemoteClient
{
    std::shared_ptr<User> u;
    bool isConn;
public:
    PackageBuffer *incompletePackageBuffer;
    BUFF_SIZE incompletePackageFullLength;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    RemoteClient()
    {
        isConn = false;
        incompletePackageBuffer = new PackageBuffer(nullptr, 0);
        incompletePackageFullLength = 0;
    }
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    ~RemoteClient()
    {
        delete incompletePackageBuffer;
    }
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    void connectUser(std::shared_ptr<User> _u)
    {
        u = _u;
        isConn = true;
    }
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    void releaseUser()
    {
        isConn = false;
        delete incompletePackageBuffer;
        incompletePackageBuffer = new PackageBuffer(nullptr, 0);
        incompletePackageFullLength = 0;
    }
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    bool isConnected()const
    {
        return isConn;
    }
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    std::shared_ptr<User> getUser()
    {
        return u;
    }
};

#endif // CLIENTSOCKET_H
