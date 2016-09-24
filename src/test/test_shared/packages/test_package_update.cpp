#ifndef PACKAGE_UPDATE_H
#define PACKAGE_UPDATE_H

#include "../shared/package.h"
#include "../shared/package_buffer.h"

/**
\struct
\brief
*/
struct PackageUpdate : PackageMultiPackage
{
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

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    PackageUpdate(PackageBuffer *buf):
        PackageMultiPackage(buf)
    {
        if (buf == nullptr)
        {
            checkIfEnoughSize(this, 0);
        }
        checkIfEnoughSize(this, buf->getLength());
    }

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
    virtual ~PackageUpdate() {}
};

/**
\struct
\brief
*/
struct PackageUpdateBlackList : PackageUpdate
{
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageUpdateBlackList(PackageBuffer *buf):
        PackageUpdate(buf)
    {}

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~PackageUpdateBlackList(){}
};
/**
\struct
\brief
*/
struct PackageUpdateFriendStatus : PackageUpdate
{
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageUpdateFriendStatus(PackageBuffer *buf):
        PackageUpdate(buf)
    {}

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~PackageUpdateFriendStatus(){}
};
/**
\struct
\brief
*/
struct PackageUpdateRoomPresence : PackageUpdate
{
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageUpdateRoomPresence(PackageBuffer *buf):
        PackageUpdate(buf)
    {}

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~PackageUpdateRoomPresence(){}
};
/**
\struct
\brief
*/
struct PackageUpdateBlackListPresence : PackageUpdate
{
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageUpdateBlackListPresence(PackageBuffer *buf):
        PackageUpdate(buf)
    {}

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~PackageUpdateBlackListPresence(){}
};
#endif // PACKAGE_UPDATE_H
