/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <mutex>

#include "../shared/messages/message.h"

/**
\struct
\brief
*/
struct Context
{

};

/**
\struct
\brief
*/
struct MessageContext : Context
{
    MessageProcessable * m;

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
struct CryptoContext : Context
{
    PackageWrapper * m;

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

    /**
    \see task.h
    */
    virtual void execute();
};
