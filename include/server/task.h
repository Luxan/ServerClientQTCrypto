/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <mutex>

#include "../shared/messages/message.h"
#include "../shared/crypto/cipher.h"
#include "../shared/crypto/key_agreement_agent.h"

/**
\struct
\brief
*/
class Context
{
public:
    virtual ~Context(){}
};

/**
\struct
\brief
*/
class MessageContext : public Context
{
private:
    MessageProcessable * m;
public:
    MessageProcessable * extractMessage()
    {
        MessageProcessable * msg = m;
        m = nullptr;
        return msg;
    }

    void setMessage(MessageProcessable * msg)
    {
        if (m != nullptr)
            delete m;

        m = msg;
    }
    MessageContext(MessageProcessable * m):
        m(m)
    {}
    /**
    \brief
    \pre
    \post
    */
    virtual ~MessageContext();
};

/**
\struct
\brief
*/
class MessageKeyAgreementContext : public MessageContext
{
private:
    KeyAgreamentAgent * agent;
public:
    KeyAgreamentAgent * getAgent()
    {
        return agent;
    }

    MessageKeyAgreementContext(MessageProcessable * m, KeyAgreamentAgent * agent):
        MessageContext(m), agent(agent)
    {}

    virtual ~MessageKeyAgreementContext(){}
};

/**
\struct
\brief
*/
class CryptoContext : public Context
{
private:
    PackageWrapper * m;
    Cipher * cipher;
    bool encrypt;
public:
    CryptoContext(PackageWrapper * m, Cipher * cipher, bool encrypt):
        m(m), cipher(cipher), encrypt(encrypt)
    {}

    PackageWrapper * extractWrapper()
    {
        PackageWrapper * w = m;
        m = nullptr;
        return w;
    }

    bool isEncryption()
    {
        return encrypt;
    }

    Cipher * extractCipher()
    {
        Cipher * w = cipher;
        cipher = nullptr;
        return w;
    }

    void setCipher(Cipher * ciph)
    {
        cipher = ciph;
    }

    void setWrapper(PackageWrapper * msg)
    {
        m = msg;
    }

    /**
    \brief
    \pre
    \post
    */
    virtual ~CryptoContext();
};

/**
\class
\brief
*/
class Task
{
protected:
    Context * context;
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    Task(Context *_context);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual void execute() = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual ~Task();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    Context * getContext();
};

/**
\class
\brief
*/
class MessageTask : public Task
{
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    MessageTask(MessageContext * c):
        Task(c)
    {}

    /**
    \see task.h
    */
    virtual void execute();

    virtual ~MessageTask(){}
};

class MessageKeyAgreementTask : public MessageTask
{
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    MessageKeyAgreementTask(MessageKeyAgreementContext * c):
        MessageTask(c)
    {}
    /**
    \see task.h
    */
    virtual void execute();

    virtual ~MessageKeyAgreementTask(){}
};


/**
\class
\brief
*/
class CryptoTask : public Task
{
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    CryptoTask(CryptoContext * c):
        Task(c)
    {}

    Package * createPackage(PackageBuffer * buf);

    void encrypt(PackageWrapper ** pw, Cipher * cipher);

    void decrypt(PackageWrapper ** pw, Cipher *cipher);
    /**
    \see task.h
    */
    virtual void execute();

    virtual ~CryptoTask(){}
};
