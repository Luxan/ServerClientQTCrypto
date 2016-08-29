#pragma once

#include "../shared/package.h"
#include "../shared/status.h"

/**
\struct
\brief
*/
struct PackageInstantResponse
{

};

/**
\struct
\brief
*/
struct PackagePing : PackageStrictSize, PackageInstantResponse
{
    Status status;
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
        return sizeof(status);
    }
    PackagePing(Status status):
        status(status)
    {}
    PackagePing()
    {}
};

/**
\struct
\brief
*/
struct PackageRequestPublicKey : PackageStrictSize, PackageInstantResponse
{
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
        return 0;
    }
};

//-----------------------------------
/**
\struct
\brief
*/
struct PackageRequestLogin : PackageInstantResponse, PackageDynamicSize
{
    uint8_t loginSize;
    uint8_t passwordSize;
    uint8_t *login;
    uint8_t *password;

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    size_t realSize()const
    {
        return sizeof(loginSize) + sizeof(passwordSize) + loginSize + passwordSize;
    }

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageRequestLogin(PackageBuffer *buf)
    {
        if (buf == nullptr)
        {
            checkIfEnoughSize(this, 0);
        }

        checkIfEnoughSize(this, buf->getLength());

        buf->fillBuffer(&loginSize, sizeof(loginSize));

        login = new uint8_t[loginSize + 1];
        buf->fillBuffer(login, loginSize);

        buf->fillBuffer(&passwordSize, sizeof(passwordSize));

        password = new uint8_t[passwordSize + 1];
        buf->fillBuffer(password, passwordSize);

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
    PackageRequestLogin(const std::string &_login, const std::string &_password)
    {
        loginSize = _login.size();
        passwordSize = _password.size();
        login = new uint8_t[loginSize + 1];
        password = new uint8_t[passwordSize + 1];
        memcpy(login, _login.c_str(), _login.size());
        memcpy(password, _password.c_str(), _password.size());
    }

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    size_t minSize() const
    {
        return sizeof(loginSize) + sizeof(login)
               + sizeof(passwordSize) + sizeof(password);
    }

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    ~PackageRequestLogin()
    {
        delete[] login;
        delete[] password;
    }
};
