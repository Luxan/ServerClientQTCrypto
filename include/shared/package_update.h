#pragma once

#include <memory>

#include "../shared/package.h"
#include "../shared/package_buffer.h"
#include "../shared/id_client.h"
#include "../shared/user.h"

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
struct PackageResponseUserDetails : PackageUpdate
{
    uint8_t nameLength;
    Status status;
    ClientID id;
    uint8_t * name;

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
        return sizeof(nameLength) + sizeof(status) + sizeof(id) + 1;
    }
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageResponseUserDetails(PackageBuffer *buf):
        PackageUpdate(buf)
    {
        if (buf == nullptr)
        {
            checkIfEnoughSize(this, 0);
        }

        checkIfEnoughSize(this, buf->getLength());

        buf->fillBuffer(&nameLength, sizeof(nameLength));
        buf->fillBuffer((uint8_t*)&status, sizeof(status));
        buf->fillBuffer((uint8_t*)&id, sizeof(id));

        name = new uint8_t[nameLength + 1];
        buf->fillBuffer(name, nameLength);

        delete buf;
    }
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageResponseUserDetails(std::shared_ptr<User> u):
        PackageUpdate(new PackageBuffer((uint8_t*)nullptr, 0))
    {
        uint8_t size = u->getName().length();

        this->buff->concatBuff(&size, sizeof(size));
        this->buff->concatBuff((uint8_t*)&u->getStatus(), sizeof(Status));
        this->buff->concatBuff((uint8_t*)&u->getID(), sizeof(ClientID));
        this->buff->concatBuff((uint8_t*)u->getName().c_str(), size);
    }
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
