#include "../../include/server/controllers/controller_gui_encryptionprocessor.h"
#include "../../include/server/mainwindow.h"
#include "../../include/server/modules/crypto_processor.h"
#include "../../include/server/impulse.h"
#include "../../include/server/slog.h"

void Controller_GUI_EncryptionProcessor::CheckModule1Events(void *module1, void *module2)
{
    MainWindow *mainWindow = (MainWindow *)module1;
    EncryptionProcessor *eventGiver = (EncryptionProcessor *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::ResponseStartEncryptionProcessor:
            SLog::logInfo() << "Database Started.";
            mainWindow->setChildEnabled(ChildController::EnabledFlag::EncryptionProcessor, true);
            if (mainWindow->isAllChildsEnabled())
            {
                mainWindow->enableGui();
            }

            deleteAndNext = true;
            break;
        case eSystemEvent::ResponseSleepEncryptionProcessor:
            SLog::logInfo() << "Database Stopped.";
            mainWindow->setChildEnabled(ChildController::EnabledFlag::EncryptionProcessor, false);
            if (mainWindow->isAllChildsDisabled())
            {
                mainWindow->enableGui();
            }

            deleteAndNext = true;
            break;
        case eSystemEvent::ErrorEncryptionProcessor:
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

void Controller_GUI_EncryptionProcessor::CheckModule2Events(void *module1, void * module2)
{
    EncryptionProcessor *encp = (EncryptionProcessor *)module1;
    MainWindow *eventGiver = (MainWindow *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::RequestStartEncryptionProcessor:
            encp->RequestStart();
            deleteAndNext = true;
            break;
        case eSystemEvent::RequestSleepEncryptionProcessor:
            encp->RequestStop();
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