#include "../../include/server/controllers/controller_gui_decryptionprocessor.h"
#include "../../include/server/mainwindow.h"
#include "../../include/server/modules/crypto_processor.h"
#include "../../include/server/impulse.h"
#include "../../include/server/slog.h"

void Controller_GUI_DecryptionProcessor::CheckModule1Events(void *module1, void *module2)
{
    MainWindow *mainWindow = (MainWindow *)module1;
    DecryptionProcessor *eventGiver = (DecryptionProcessor *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::ResponseStartDecryptionProcessor:
            SLog::logInfo() << "DecryptionProcessor Started.";
            mainWindow->setChildEnabled(ChildController::EnabledFlag::DecryptionProcessor, true);
            if (mainWindow->isAllChildsEnabled())
            {
                mainWindow->enableGui();
            }

            deleteAndNext = true;
            break;
        case eSystemEvent::ResponseSleepDecryptionProcessor:
            SLog::logInfo() << "DecryptionProcessor Stopped.";
            mainWindow->setChildEnabled(ChildController::EnabledFlag::DecryptionProcessor, false);
            if (mainWindow->isAllChildsDisabled())
            {
                mainWindow->enableGui();
            }

            deleteAndNext = true;
            break;
        case eSystemEvent::ErrorDecryptionProcessor:
            SLog::logError() << "DecryptionProcessor Error: " + ((ImpulseError *)i)->getError();
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

void Controller_GUI_DecryptionProcessor::CheckModule2Events(void *module1, void * module2)
{
    DecryptionProcessor *decp = (DecryptionProcessor *)module1;
    MainWindow *eventGiver = (MainWindow *)module2;
    Impulse *i = nullptr;
    Impulse *todelete = nullptr;
    bool deleteAndNext = false;

    i = eventGiver->getNextImpulse(i);
    while (i)
    {
        switch (i->getEvent())
        {
        case eSystemEvent::RequestStartDecryptionProcessor:
            decp->RequestStart();
            deleteAndNext = true;
            break;
        case eSystemEvent::RequestSleepDecryptionProcessor:
            decp->RequestStop();
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

void Controller_GUI_DecryptionProcessor::setMainWindowObj(MainWindow * module)
{
    setModule1Obj(module);
}

void Controller_GUI_DecryptionProcessor::setDecryptionProcessorObj(DecryptionProcessor * module)
{
    setModule2Obj(module);
}
