/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef IMPULSE_H
#define IMPULSE_H

#include <string>

#include "../server/systemevents.h"
#include "../shared/message.h"
#include "../shared/package.h"

class Message;

/**
\class
\brief
*/
class Impulse
{
    eSystemEvent e;
    std::mutex lock;
    bool inInProcessingState;
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    Impulse(eSystemEvent _e):
        e(_e), inInProcessingState(false)
    {}

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    bool isInProcessingState()
    {
        std::lock_guard<std::mutex> guard(lock);
        return inInProcessingState;
    }

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    void setProcessingState(bool state)
    {
        std::lock_guard<std::mutex> guard(lock);
        inInProcessingState = state;
    }

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    eSystemEvent getEvent()
    {
        return e;
    }
};

class ImpulseSignal : public Impulse
{
    const std::thread::id id;
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ImpulseSignal(eSystemEvent e, const std::thread::id _id):
        Impulse(e), id(_id)
    {}
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ImpulseSignal(eSystemEvent e):
        Impulse(e), id(0)
    {}

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    const std::thread::id getThreadID() const
    {
        return id;
    }
};

/**
\class
\brief
*/
class ImpulseError : public Impulse
{
    std::string error;
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    std::string getError()
    {
        return error;
    }
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ImpulseError(eSystemEvent e, std::string _error):
        Impulse(e), error(_error)
    {}
};

/**
\class
\brief
*/
class ImpulseMessage : public Impulse
{
    Message * m;
public:
    /**
    \see impulse.h
    */
    Message * getData()
    {
        return m;
    }
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ImpulseMessage(eSystemEvent e, Message * _m):
        Impulse(e), m(_m)
    {}
};

/**
\class
\brief
*/
class ImpulsePackage : public Impulse
{
    PackageWrapper * pw;
public:
    /**
    \see impulse.h
    */
    PackageWrapper * getData()
    {
        return pw;
    }
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ImpulsePackage(eSystemEvent e, PackageWrapper * p):
        Impulse(e), pw(p)
    {}
};

#endif // IMPULSE_H

