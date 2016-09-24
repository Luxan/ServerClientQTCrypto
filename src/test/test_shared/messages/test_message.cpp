/**
\author Sergey Gorokh (ESEGORO)
*/
#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>
#include <list>

#include "../shared/package.h"
#include "../shared/id_client.h"
#include "../shared/log.h"

/**
\class
\brief
*/
class Message
{
protected:
    std::string data;
    std::list<Package *> lPackages;
    ClientID senderid;
    ClientID recieverid;
    PackageWrapper::ePackageType packageType;
public:
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    PackageWrapper::ePackageType getPackageType()const
    {
        return packageType;
    }

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    ClientID getSenderID()
    {
        return senderid;
    }

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    ClientID getReceiverID()
    {
        return recieverid;
    }

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    Message(Package * p)
    {
        lPackages.push_back(p);
    }

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~Message()
    {
        for (Package * p : lPackages)
            delete p;
    }

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    std::list<Package*> &splitIntoPackages()
    {
        return lPackages;
    }

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    std::string GetString();
};

/**
\class
\brief
*/
class MessageProcessable : Message
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
    MessageProcessable(Package * p):
        Message(p)
    {}
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual void process() = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool isReadyToProcess();

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~MessageProcessable()
    {

    }
};

/**
\class
\brief
*/
class MessageRequestAutocomplete : public MessageProcessable
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
    MessageRequestAutocomplete(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageRequestAutocomplete";}
};
/**
\class
\brief
*/
class MessageRequestLeavingRoom : public MessageProcessable
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
    MessageRequestLeavingRoom(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageRequestLeavingRoom";}
};
/**
\class
\brief
*/
class MessageRequestMessageHistory : public MessageProcessable
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
    MessageRequestMessageHistory(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageRequestMessageHistory";}
};
/**
\class
\brief
*/
class MessageRequestJoinRoom : public MessageProcessable
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
    MessageRequestJoinRoom(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageRequestJoinRoom";}
};
/**
\class
\brief
*/
class ResponseAutocompleteMessage : public MessageProcessable
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
    ResponseAutocompleteMessage(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "ResponseAutocompleteMessage";}
};
/**
\class
\brief
*/
class MessageResponseLeavingRoom : public MessageProcessable
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
    MessageResponseLeavingRoom(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageResponseLeavingRoom";}
};
/**
\class
\brief
*/
class MessageResponseJoinRoom : public MessageProcessable
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
    MessageResponseJoinRoom(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageResponseJoinRoom";}
};
/**
\class
\brief
*/
class MessageUpdateBlackList : public MessageProcessable
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
    MessageUpdateBlackList(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageUpdateBlackList";}
};
/**
\class
\brief
*/
class MessageUpdateFriendStatus : public MessageProcessable
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
    MessageUpdateFriendStatus(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageUpdateFriendStatus";}
};
/**
\class
\brief
*/
class MessageUpdateRoomPresence : public MessageProcessable
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
    MessageUpdateRoomPresence(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageUpdateRoomPresence";}
};
/**
\class
\brief
*/
class MessageUpdateBlackListPresence : public MessageProcessable
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
    MessageUpdateBlackListPresence(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageUpdateBlackListPresence";}
};
/**
\class
\brief
*/
class MessageUserMessage : public MessageProcessable
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
    MessageUserMessage(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageUserMessage";}
};
/**
\class
\brief
*/
class MessageUserNotification : public MessageProcessable
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
    MessageUserNotification(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageUserNotification";}
};
/**
\class
\brief
*/
class MessageReceipeMessageHistory : public MessageProcessable
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
    MessageReceipeMessageHistory(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageReceipeMessageHistory";}
};
/**
\class
\brief
*/
class MessagePTPhistory : public MessageProcessable
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
    MessagePTPhistory(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessagePTPhistory";}
};
/**
\class
\brief
*/
class MessageSetUserInBlackList : public MessageProcessable
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
    MessageSetUserInBlackList(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageSetUserInBlackList";}
};
/**
\class
\brief
*/
class MessageUnFriendUser : public MessageProcessable
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
    MessageUnFriendUser(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageUnFriendUser";}
};
/**
\class
\brief
*/
class MessagePing : public MessageProcessable
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
    MessagePing(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessagePing";}
};
/**
\class
\brief
*/
class MessageError : public MessageProcessable
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
    MessageError(Package * p):
        MessageProcessable(p)
    {}
    /**
    \see message.h
    */
    void process() {}//SLog::logInfo() << "MessageError";}
};

#endif // MESSAGE_H
