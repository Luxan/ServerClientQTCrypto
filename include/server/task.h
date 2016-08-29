/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef TASK_H
#define TASK_H

#include <mutex>

#include "../shared/message.h"

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
    void execute();
};

#endif // TASK_H
