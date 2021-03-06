#pragma once

#include "../../shared/packages/package.h"
#include "../../shared/crypto/key.h"
#include "../../shared/id_client.h"
#include "../../shared/id_message.h"
#include "../../shared/id_room.h"
#include "../../shared/id_session.h"
#include "../../shared/error_enum.h"

struct PackageSessionID : PackageStrictSize
{
    SessionID id;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageSessionID() {}
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageSessionID(SessionID id):
        id(id)
    {}
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    size_t strictSize() const
    {
        return sizeof(id);
    }

    virtual PackageBuffer * toPackageBuffer()
    {
        return new PackageBuffer((uint8_t*)&id, sizeof(id));
    }
};

/**
\struct
\brief
*/
struct PackageRequestUserDetails : PackageStrictSize
{
    ClientID id;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageRequestUserDetails() {}
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageRequestUserDetails(ClientID id):
        id(id)
    {}
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    size_t strictSize() const
    {
        return sizeof(id);
    }
};

/**
\struct
\brief
*/
struct PackageResponseLogin : PackageStrictSize
{
    ClientID id;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    PackageResponseLogin() {}
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    PackageResponseLogin(ClientID id):
        id(id)
    {}
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    size_t strictSize() const
    {
        return sizeof(id);
    }
};

/**
\struct
\brief
*/
struct PackageError : PackageStrictSize
{
    uint8_t errorNumber;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    PackageError() {}
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    PackageError(uint8_t errorNumber)
    {
        this->errorNumber = errorNumber;
    }

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    size_t strictSize() const
    {
        return sizeof(errorNumber);
    }
};
/**
\struct
\brief
*/
struct PackageRequestJoinRoom : PackageStrictSize
{
    RoomID id;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    size_t strictSize() const
    {
        return sizeof(id);
    }
};
/**
\struct
\brief
*/
struct PackageRequestLeavingRoom : PackageStrictSize
{
    RoomID id;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    size_t strictSize() const
    {
        return sizeof(id);
    }
};
/**
\struct
\brief
*/
struct PackageSetUserInBlackList : PackageStrictSize
{
    ClientID id;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    size_t strictSize() const
    {
        return sizeof(id);
    }
};

/**
\struct
\brief
*/
struct PackageRequestMessageHistory : PackageStrictSize
{
    MessageID id;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    size_t strictSize() const
    {
        return sizeof(id);
    }
};


/**
\struct
\brief
*/
struct PackageUnFriendUser : PackageStrictSize
{
    ClientID id;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    size_t strictSize() const
    {
        return sizeof(id);
    }
};
