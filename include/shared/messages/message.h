/**
\author Sergey Gorokh (ESEGORO)
*/
#pragma once

#include <string>
#include <vector>
#include <list>
#include <time.h>

#include "../packages/package.h"
#include "../packages/package_instant_replay.h"
#include "../id_client.h"
#include "../id_message.h"
#include "../id_room.h"
#include "../log.h"

/**
\class
\brief
*/
class Message
{
protected:
    time_t time_received;
    time_t time_sended;

    MessageID message_id;
    SessionID sessionID;

    const PackageWrapper::ePackageType packageType;
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    SessionID getSessionID() const
    {
        return sessionID;
    }
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
    Message(PackageWrapper::ePackageType type):
        packageType(type)
    {}

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
class MessageMulty : public Message
{
protected:
    //xml
    std::string data;
    std::list<Package *> lPackages;
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual void extractData() = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual void splitDataToPackages() = 0;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    std::list<Package *> * extractPackages()
    {
        std::list<Package *> * ret = new std::list<Package *>(lPackages);
        lPackages.clear();
        return ret;
    }
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    MessageMulty(Package * package, PackageWrapper::ePackageType type):
        Message(type)
    {
        lPackages.push_back(package);
    }
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    virtual ~MessageMulty()
    {
        for (Package * p : lPackages)
        {
            delete p;
        }
    }
};

/**
\class
\brief
*/
class MessageSingle : public Message
{
protected:
    Package * package;
public:
    MessageSingle(Package * package, PackageWrapper::ePackageType type):
        Message(type), package(package)
    {}
    Package * extractPackage()
    {
        Package * ret = package;
        package = nullptr;
        return ret;
    }
    virtual ~MessageSingle()
    {
        if (package != nullptr)
            delete package;
    }
};

/**
\class
\brief
*/
class MessageProcessable
{
protected:

public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    MessageProcessable(){}
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
    bool isReadyToProcess(){}

	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~MessageProcessable(){}
};

class MessageSessionDetailResponse : public MessageProcessable, public MessageSingle
{
    Key * rsaPublicKey;
public:

    MessageSessionDetailResponse(PackageSessionDetailResponse * p, Key * rsaPublicKey):
        MessageSingle(p, PackageWrapper::ePackageType::SessionDetailResponse), rsaPublicKey(rsaPublicKey)
    {}

    Key * getRsaPublicKey()
    {
        return rsaPublicKey;
    }

    void process() {}//LOG_INFO("MessageRequestAutocomplete";}
};

class MessageSessionDetailRequest : public MessageProcessable, public MessageSingle
{
public:
    Buffer * getP()
    {
        return ((PackageSessionDetailRequest*)extractPackage())->p;
    }

    Buffer * getQ()
    {
        return ((PackageSessionDetailRequest*)extractPackage())->q;
    }

    Buffer * getG()
    {
        return ((PackageSessionDetailRequest*)extractPackage())->g;
    }

    Key * getRsaPublicKey()
    {
        return ((PackageSessionDetailRequest*)extractPackage())->rsaPublicKey;
    }

    MessageSessionDetailRequest(PackageSessionDetailRequest * p):
        MessageSingle(p, PackageWrapper::ePackageType::SessionDetailRequest)
    {}

    void process() {}//LOG_INFO("MessageRequestAutocomplete";}
};

/**
\class
\brief
*/
class MessagePing : public MessageProcessable, public MessageSingle
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
    MessagePing(PackagePing * p):
        MessageSingle(p, PackageWrapper::ePackageType::Ping)
    {}

    std::string getLogin()
    {
        PackagePing * ping = (PackagePing*)extractPackage();
        std::string str((char*)ping->login->getPointerToBuffer());
        str.resize(ping->login->getLength());
        return str;
    }

    Hash * getSaltedPassword()
    {
        return ((PackagePing*)extractPackage())->saltedPassword;
    }

    Status getstatus()
    {
        return ((PackagePing*)extractPackage())->status;
    }

    /**
    \see message.h
    */
    void process() {}//LOG_INFO("MessagePing";}
};
///**
//\class
//\brief
//*/
//class MessageRequestAutocomplete : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageRequestAutocomplete(PackageRequestAutocomplete * p):
//        MessageProcessable(p, PackageWrapper::ePackageType::RequestAutocomplete)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageRequestAutocomplete";}
//};
///**
//\class
//\brief
//*/
//class MessageRequestLeavingRoom : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageRequestLeavingRoom(PackageRequestLeavingRoom * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageRequestLeavingRoom";}
//};
///**
//\class
//\brief
//*/
//class MessageRequestMessageHistory : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageRequestMessageHistory(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageRequestMessageHistory";}
//};
///**
//\class
//\brief
//*/
//class MessageRequestJoinRoom : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageRequestJoinRoom(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageRequestJoinRoom";}
//};
///**
//\class
//\brief
//*/
//class ResponseAutocompleteMessage : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    ResponseAutocompleteMessage(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("ResponseAutocompleteMessage";}
//};
///**
//\class
//\brief
//*/
//class MessageResponseLeavingRoom : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageResponseLeavingRoom(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageResponseLeavingRoom";}
//};
///**
//\class
//\brief
//*/
//class MessageResponseJoinRoom : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageResponseJoinRoom(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageResponseJoinRoom";}
//};
///**
//\class
//\brief
//*/
//class MessageUpdateBlackList : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageUpdateBlackList(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageUpdateBlackList";}
//};
///**
//\class
//\brief
//*/
//class MessageUpdateFriendStatus : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageUpdateFriendStatus(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageUpdateFriendStatus";}
//};
///**
//\class
//\brief
//*/
//class MessageUpdateRoomPresence : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageUpdateRoomPresence(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageUpdateRoomPresence";}
//};
///**
//\class
//\brief
//*/
//class MessageUpdateBlackListPresence : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageUpdateBlackListPresence(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageUpdateBlackListPresence";}
//};
///**
//\class
//\brief
//*/
//class MessageUserMessage : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageUserMessage(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageUserMessage";}
//};
///**
//\class
//\brief
//*/
//class MessageUserNotification : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageUserNotification(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageUserNotification";}
//};
///**
//\class
//\brief
//*/
//class MessageReceipeMessageHistory : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageReceipeMessageHistory(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageReceipeMessageHistory";}
//};
///**
//\class
//\brief
//*/
//class MessagePTPhistory : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessagePTPhistory(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessagePTPhistory";}
//};
///**
//\class
//\brief
//*/
//class MessageSetUserInBlackList : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageSetUserInBlackList(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageSetUserInBlackList";}
//};
///**
//\class
//\brief
//*/
//class MessageUnFriendUser : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageUnFriendUser(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageUnFriendUser";}
//};
///**
//\class
//\brief
//*/
//class MessageError : public MessageProcessable
//{
//public:
//    /**
//    \param
//    \return
//    \throw
//    \brief
//    \pre
//    \post
//    */
//    MessageError(Package * p):
//        MessageProcessable(p)
//    {}
//    /**
//    \see message.h
//    */
//    void process() {}//LOG_INFO("MessageError";}
//};
