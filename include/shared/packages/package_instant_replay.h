#pragma once

#include "package.h"
#include "../status.h"
#include "../crypto/hash.h"
#include "../crypto/key.h"
#include "../crypto/certificate.h"
#include "../id_session.h"
/**
\struct
\brief
*/
struct PackagePing : Package
{
    Buffer * login;
    Hash * saltedPassword;
    Status status;

    size_t size()const
    {
        return sizeof(status) + sizeof(uint8_t) + login->getLength() +  sizeof(uint8_t) + saltedPassword->getLength();
    }

    PackagePing(std::string login, Hash * saltedPassword, Status status):
        login(new PackageBuffer((const uint8_t *)login.c_str(), login.length())), saltedPassword(saltedPassword), status(status)
    {}

    PackagePing(PackageBuffer * buf)
    {
        BUFF_SIZE size;
        buf->fillBuffer((uint8_t*)&size, sizeof(BUFF_SIZE));
        login = new Buffer();
        login->resize(size);
        buf->fillBuffer(login->getPointerToBuffer(), login->getLength());

        buf->fillBuffer((uint8_t*)&size, sizeof(BUFF_SIZE));
        saltedPassword = new Hash();
        saltedPassword->resize(size);
        buf->fillBuffer(saltedPassword->getPointerToBuffer(), saltedPassword->getLength());

        buf->fillBuffer((uint8_t*)&status, sizeof(status));
    }

    virtual ~PackagePing()
    {
        delete login;
    }

    virtual PackageBuffer * toPackageBuffer()
    {
        BUFF_SIZE size = login->getLength();
        PackageBuffer * buff = new PackageBuffer((uint8_t*)(&size), sizeof(size));
        buff->concatBuff(login->getPointerToBuffer(), login->getLength());

        size = saltedPassword->getLength();
        buff->concatBuff((uint8_t*)(&size), sizeof(size));
        buff->concatBuff(saltedPassword->getPointerToBuffer(), saltedPassword->getLength());

        buff->concatBuff((uint8_t*)(&status), sizeof(status));

        return buff;
    }
};

/**
\struct
\brief
*/
struct PackageSessionDetailRequest : PackageDynamicSize
{
    Buffer * p, * g, * q;
    Key * rsaPublicKey;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageSessionDetailRequest(Buffer * p, Buffer * g, Buffer * q, Key * rsaPublicKey):
        p(p), g(g), q(q), rsaPublicKey(rsaPublicKey)
    {}

    PackageSessionDetailRequest(PackageBuffer *buf)
    {
        BUFF_SIZE size;
        buf->fillBuffer((uint8_t*)&size, sizeof(BUFF_SIZE));
        p = new Buffer();
        p->resize(size);
        buf->fillBuffer(p->getPointerToBuffer(), p->getLength());
        buf->fillBuffer((uint8_t*)&size, sizeof(BUFF_SIZE));
        g = new Buffer();
        g->resize(size);
        buf->fillBuffer(g->getPointerToBuffer(), g->getLength());
        buf->fillBuffer((uint8_t*)&size, sizeof(BUFF_SIZE));
        q = new Buffer();
        q->resize(size);
        buf->fillBuffer(q->getPointerToBuffer(), q->getLength());
        buf->fillBuffer((uint8_t*)&size, sizeof(BUFF_SIZE));
        rsaPublicKey = new Key(size, size);
        rsaPublicKey->resize(size);
        buf->fillBuffer(rsaPublicKey->getPointerToBuffer(), rsaPublicKey->getLength());
    }

    virtual BUFF_SIZE size() const
    {
        return sizeof(BUFF_SIZE) + p->getLength() + sizeof(BUFF_SIZE) + g->getLength() + sizeof(BUFF_SIZE) + q->getLength();
    }

    virtual PackageBuffer * toPackageBuffer()
    {
        BUFF_SIZE size = p->getLength();
        PackageBuffer * buff = new PackageBuffer((uint8_t*)(&size), sizeof(size));
        buff->concatBuff(p->getPointerToBuffer(), p->getLength());

        size = g->getLength();
        buff->concatBuff((uint8_t*)(&size), sizeof(size));
        buff->concatBuff(g->getPointerToBuffer(), g->getLength());

        size = q->getLength();
        buff->concatBuff((uint8_t*)(&size), sizeof(size));
        buff->concatBuff(q->getPointerToBuffer(), q->getLength());

        size = rsaPublicKey->getLength();
        buff->concatBuff((uint8_t*)(&size), sizeof(size));
        buff->concatBuff(rsaPublicKey->getBuff(), rsaPublicKey->getLength());

        return buff;
    }
};

/**
\struct
\brief
*/
struct PackageSessionDetailResponse : PackageDynamicSize
{
    u_int32_t session_id;
    Certificate * certificate;
    uint32_t agreedValueLength;
    Key * staticPublicKey, * ephemeralPublicKey;

    virtual BUFF_SIZE size()const
    {
        return sizeof(session_id) +
                certificate->getBuffer()->getLength() + sizeof(BUFF_SIZE) +
                sizeof(agreedValueLength) +
                staticPublicKey->getKeyLength() + sizeof(BUFF_SIZE) +
                ephemeralPublicKey->getKeyLength() + sizeof(BUFF_SIZE);
    }

    PackageSessionDetailResponse(PackageBuffer *buf)
    {
        BUFF_SIZE size;
        buf->fillBuffer((uint8_t*)&size, sizeof(BUFF_SIZE));
        Buffer *b = new Buffer();
        b->resize(size);
        buf->fillBuffer(b->getPointerToBuffer(), b->getLength());
        certificate = new Certificate(b);

        buf->fillBuffer((uint8_t*)&agreedValueLength, sizeof(agreedValueLength));

        buf->fillBuffer((uint8_t*)&size, sizeof(BUFF_SIZE));
        staticPublicKey = new Key(size, size);
        staticPublicKey->resize(size);
        buf->fillBuffer(staticPublicKey->getPointerToBuffer(), staticPublicKey->getLength());

        buf->fillBuffer((uint8_t*)&size, sizeof(BUFF_SIZE));
        ephemeralPublicKey = new Key(size, size);
        ephemeralPublicKey->resize(size);
        buf->fillBuffer(ephemeralPublicKey->getPointerToBuffer(), ephemeralPublicKey->getLength());
    }

    PackageSessionDetailResponse(Certificate *cert, uint32_t agreedValueLength, Key * staticPublicKey, Key * ephemeralPublicKey):
        certificate(cert), agreedValueLength(agreedValueLength), staticPublicKey(staticPublicKey), ephemeralPublicKey(ephemeralPublicKey)
    {}

    virtual PackageBuffer * toPackageBuffer()
    {
        PackageBuffer * buff = new PackageBuffer((uint8_t*)(&session_id), sizeof(session_id));
        BUFF_SIZE size = certificate->getBuffer()->getLength();
        buff->concatBuff((uint8_t*)(&size), sizeof(size));
        buff->concatBuff(certificate->getBuffer()->getPointerToBuffer(), certificate->getBuffer()->getLength());

        buff->concatBuff((uint8_t*)(&agreedValueLength), sizeof(agreedValueLength));

        size = staticPublicKey->getLength();
        buff->concatBuff((uint8_t*)(&size), sizeof(size));
        buff->concatBuff(staticPublicKey->getPointerToBuffer(), staticPublicKey->getLength());

        size = ephemeralPublicKey->getLength();
        buff->concatBuff((uint8_t*)(&size), sizeof(size));
        buff->concatBuff(ephemeralPublicKey->getPointerToBuffer(), ephemeralPublicKey->getLength());

        return buff;
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
struct PackageRequestLogin : PackageDynamicSize
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
