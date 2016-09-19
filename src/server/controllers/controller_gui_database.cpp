/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../include/server/controllers/controller_gui_database.h"
#include "../../include/server/mainwindow.h"
#include "../../include/server/modules/database.h"
#include "../../include/server/impulse.h"
#include "../../include/server/slog.h"
#include "../../include/shared/user_credentials.h"

void Controller_GUI_Database::CheckModule1Events(void *module1, void *module2)
{
    MainWindow *mainWindow = (MainWindow *)module1;
    DataBase *eventGiver = (DataBase *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::ResponseStartDatabase:
            SLog::logInfo() << "Database Started.";
            mainWindow->setChildEnabled(ChildController::EnabledFlag::Database, true);
            if (mainWindow->isAllChildsEnabled())
            {
                mainWindow->enableGui();
            }

            deleteAndNext = true;
            break;
        case eSystemEvent::ResponseSleepDatabase:
            SLog::logInfo() << "Database Stopped.";
            mainWindow->setChildEnabled(ChildController::EnabledFlag::Database, false);
            if (mainWindow->isAllChildsDisabled())
            {
                mainWindow->enableGui();
            }

            deleteAndNext = true;
            break;
        case eSystemEvent::ErrorDatabase:
            SLog::logError() << "Database Error: " + ((ImpulseError *)i)->getError();
            deleteAndNext = true;
            break;
        case eSystemEvent::Undefined:
            SLog::logError() << "Got Undefined event!";
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

void Controller_GUI_Database::CheckModule2Events(void *module1, void * module2)
{
    DataBase *database = (DataBase *)module1;
    MainWindow *eventGiver = (MainWindow *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;
    std::string name;
    UserCredentials * c;
    ClientID id;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::DatabaseDeleteUser:
            id = ((ImpulseUserID *)i)->getData();
            database->removeUser(id);
            deleteAndNext = true;
            break;
        case eSystemEvent::DatabaseRegisterUser:
            name = ((ImpulseUserCredentialsPackage *)i)->getName();
            c = ((ImpulseUserCredentialsPackage *)i)->getCredentials();
            database->registerUserToDatabase(name, c->getID(), c->getLogin(), c->getPassword(), c->getIteration(), c->getSalt());
            deleteAndNext = true;
            break;
        case eSystemEvent::RequestStartDatabase:
            database->RequestStart();
            deleteAndNext = true;
            break;
        case eSystemEvent::RequestSleepDatabase:
            database->RequestStop();
            deleteAndNext = true;
            break;
        case eSystemEvent::Undefined:
            SLog::logError() << "Got Undefined event!";
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

void Controller_GUI_Database::setMainWindowObj(MainWindow * module)
{
    setModule1Obj(module);
}

void Controller_GUI_Database::setDataBaseObj(DataBase * module)
{
    setModule2Obj(module);
}
