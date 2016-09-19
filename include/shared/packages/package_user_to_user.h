#pragma once

#include <memory>

#include "../../shared/packages/package.h"
#include "../../shared/buffer.h"
#include "../../shared/status.h"
#include "../../shared/user.h"

/**
\struct
\brief
*/
struct PackageUserToUser : PackageMultiPackage
{
    ClientID souce;
    ClientID destination;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    PackageUserToUser(PackageBuffer *buf):
        PackageMultiPackage(buf)
    {}
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~PackageUserToUser() {}
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual size_t realSize()const
    {
        return buff->getLength();
    }
};
/**
\struct
\brief
*/
struct PackageUserMessage : PackageUserToUser
{
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageUserMessage(PackageBuffer *buf):
        PackageUserToUser(buf)
    {}
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual size_t minSize()const
    {
        return 2;
    }
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~PackageUserMessage() {}
};
/**
\struct
\brief
*/
struct PackageUserNotification : PackageUserToUser
{
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageUserNotification(PackageBuffer *buf):
        PackageUserToUser(buf)
    {}
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~PackageUserNotification() {}
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual size_t minSize()const
    {
        return 2;
    }
};
