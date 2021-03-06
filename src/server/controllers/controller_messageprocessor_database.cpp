/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../../include/server/controllers/controller_messageprocessor_database.h"
#include "../../../include/server/modules/database.h"
#include "../../../include/server/modules/message_processor.h"
#include "../../../include/server/impulse.h"
#include "../../include/server/login_server/server_logger.h"
#include "../../../include/shared/messages/message.h"

void Controller_MessageProcessor_Database::CheckModule1Events(void *module1, void *nmodule2)
{
    MessageProcessor *messageProcessor = (MessageProcessor *)module1;
    DataBase *eventGiver = (DataBase *)nmodule2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;
    Message *m;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::DatabaseMissingMessages:
            m = ((ImpulseMessage *)i)->getData();
            messageProcessor->missingMessage(m);
            deleteAndNext = true;
            break;
        case eSystemEvent::Undefined:
            LOG_ERROR("Got Undefined event!");
            deleteAndNext = true;
            break;
        default:
            break;
        }

        if (deleteAndNext)
        {
            todelete = i;
            i = eventGiver->eraseImpulseAndGetNext(i);
            delete todelete;
            deleteAndNext = false;
        }
        else
        {
            i = eventGiver->getNextImpulse(i);
        }
    }
}

void Controller_MessageProcessor_Database::CheckModule2Events(void *module1, void *module2)
{
    DataBase *database = (DataBase *)module1;
    MessageProcessor *eventGiver = (MessageProcessor *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;
    //Message *m;
    std::shared_ptr<User> u;
    ClientID id;
    UserCredentials * c;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::AttemptToLogIn:
            database->countAttempsToLogin((MessagePing*)((ImpulseMessage *)i)->getData());
            deleteAndNext = true;
            break;
        case eSystemEvent::DatabaseLoginUser:
            c = ((ImpulseUserCredentialsPackage *)i)->getCredentials();
            database->loginRequest(c->getLogin(), c->getPassword());
            deleteAndNext = true;
            break;
        case eSystemEvent::DatabaseDetermineUserSalt:
            c = ((ImpulseUserCredentialsPackage *)i)->getCredentials();
            database->saltRequest(c->getLogin());
            deleteAndNext = true;
            break;
        case eSystemEvent::DatabaseGetUserByID:
            id = ((ImpulseUserID *)i)->getData();
            database->getUserViaID(id);
            deleteAndNext = true;
            break;
        case eSystemEvent::DatabaseDeleteUser:
            id = ((ImpulseUserID *)i)->getData();
            database->removeUser(id);
            deleteAndNext = true;
            break;
        case eSystemEvent::DatabaseGetMissingMessages:
            id = ((ImpulseUserID *)i)->getData();
            //database->getm();
            deleteAndNext = true;
            break;
        case eSystemEvent::Undefined:
            LOG_ERROR("Got Undefined event!");
            deleteAndNext = true;
            break;
        default:
            break;
        }

        if (deleteAndNext)
        {
            todelete = i;
            i = eventGiver->eraseImpulseAndGetNext(i);
            delete todelete;
            deleteAndNext = false;
        }
        else
        {
            i = eventGiver->getNextImpulse(i);
        }
    }
}

void Controller_MessageProcessor_Database::setMessageProcessorObj(MessageProcessor * module)
{
    setModule1Obj(module);
}

void Controller_MessageProcessor_Database::setDataBaseObj(DataBase * module)
{
    setModule2Obj(module);
}
