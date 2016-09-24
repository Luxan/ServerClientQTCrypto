#ifndef USER_DATA_PACKAGE_H
#define USER_DATA_PACKAGE_H

#include "../shared/package.h"
#include "../shared/buffer.h"

/**
\struct
\brief
*/
struct PackageInformation : PackageMultiPackage
{
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    PackageInformation(PackageBuffer *buf):
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
    virtual ~PackageInformation() {}

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
struct PackageUserMessage : PackageInformation
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
        PackageInformation(buf)
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
struct PackageUserNotification : PackageInformation
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
        PackageInformation(buf)
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
#endif // USER_DATA_PACKAGE_H
