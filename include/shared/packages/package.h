/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <stdint.h>
#include <string>
#include <stdio.h>

#include "../package_buffer.h"
#include "../id_session.h"

/**
\struct
\brief
*/
struct Package
{
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    void thowNotEnoughSizeException(size_t gotSize, size_t expectedSize) const
    {
        if (gotSize < expectedSize)
            throw (std::string("Not enough buffer. Got: " + std::to_string(gotSize) + " Expected: " + std::to_string(expectedSize)));
    }
    virtual ~Package() {}
    virtual PackageBuffer * toPackageBuffer() = 0;
};
/**
\struct
\brief
*/
struct PackageWrapper
{
    /**
    \enum
    \brief
    */
    enum class ePackageType : uint8_t
    {
        //strict size packages
        Ping,
        SessionDetailRequest,
        SessionDetailResponse,
        SessionID,


        RequestLeavingRoom,
        RequestMessageHistory,
        SetUserInBlackList,
        UnFriendUser,
        ResponseLogin,
        Error,
        RequestJoinRoom,
        RequestUserDetails,
        //dynamic size packages
        RequestAutocomplete,
        RequestLogin,
        ResponseAutocomplete,
        ResponseUserDetails,
        UpdateBlackList,
        UpdateFriendStatus,
        UpdateRoomPresence,
        UpdateBlackListPresence,
        UserMessage,
        UserNotification
    };

    SessionID sessionID;

    PackageWrapper():
        sessionID((uint32_t)0)
    {}

    virtual ~PackageWrapper(){}
};

struct PackageWrapperEncoded : PackageWrapper
{
    PackageBuffer * buffer;

    virtual ~PackageWrapperEncoded()
    {
        if (buffer != nullptr)
            delete buffer;
    }
};

struct PackageWrapperDecoded : PackageWrapper
{
    Package *package;

    ePackageType type;

    virtual ~PackageWrapperDecoded()
    {
        if (package != nullptr)
            delete package;
    }
};

/**
\struct
\brief
*/
struct PackageStrictSize : Package
{

};

/**
\struct
\brief
*/
struct PackageDynamicSize : Package
{
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual BUFF_SIZE size()const = 0;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual void checkIfEnoughSize(Package *p, BUFF_SIZE size)const
    {
        thowNotEnoughSizeException(size, ((PackageDynamicSize*)p)->size());
    }
};


/**
\struct
\brief
*/
struct PackageMultiPackage : PackageDynamicSize
{
    uint8_t multiPackSize;
    uint8_t multiPackCurrentSize;
    PackageBuffer *buff;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    PackageMultiPackage(PackageBuffer *buf):
        buff(buf)
    {
        if (buf == nullptr)
        {
            thowNotEnoughSizeException(0, sizeof(multiPackSize) + sizeof(multiPackCurrentSize));
        }
        thowNotEnoughSizeException(buf->getLength(), sizeof(multiPackSize) + sizeof(multiPackCurrentSize));

        buf->fillBuffer(&multiPackSize, sizeof(multiPackSize));
        buf->fillBuffer(&multiPackCurrentSize, sizeof(multiPackCurrentSize));
    }

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~PackageMultiPackage()
    {
        delete buff;
    }
};

/**
\struct
\brief
*/
struct PackageRequestAutocomplete : PackageDynamicSize
{
    PackageBuffer *buff;

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
    PackageRequestAutocomplete(PackageBuffer *buf):
        buff(buf)
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
    virtual ~PackageRequestAutocomplete() {}
};
/**
\struct
\brief
*/
struct PackageResponseAutocomplete : PackageMultiPackage
{
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageResponseAutocomplete(PackageBuffer *buf):
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
    virtual size_t minSize()const
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
    virtual ~PackageResponseAutocomplete() {}
};
