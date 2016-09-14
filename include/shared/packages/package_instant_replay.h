#pragma once

#include "package.h"
#include "../status.h"
#include "../crypto/hash.h"
#include "../crypto/key.h"
#include "../crypto/certificate.h"

/**
\struct
\brief
*/
struct PackagePing : Package
{
    PackageBuffer * login;
    Hash * saltedPassword;
    Status status;

    size_t size()const
    {
        return sizeof(status) + sizeof(uint8_t) + login->getLength() +  sizeof(uint8_t) + saltedPassword->getLength();
    }
    PackagePing(std::string login, Hash * saltedPassword, Status status):
        login(login.c_str(), login.length()), saltedPassword(saltedPassword), status(status)
    {}
};

/**
\struct
\brief
*/
struct PackageSessionDetailRequest : Package
{
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    size_t size() const
    {
        return 0;
    }
};

/**
\struct
\brief
*/
struct PackageSessionDetailResponse : PackageDynamicSize
{
    Certificate * certificate;

    virtual size_t size()const
    {
        return key->getKeyLength() + certificate->getLength();
    }

    PackageSessionDetailResponse(PackageBuffer *buf)
    {
        uint8_t * b = new uint8_t[buf->getLength()];
        memcpy(b, buf->getPointerToBuffer(), buf->getLength());
        certificate = new Certificate(new Buffer(b, buf->getLength()));
    }
    PackageSessionDetailResponse(Certificate *cert)
    {
        certificate = cert;
    }
};

struct PackageLoginCheck : Package
{

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
