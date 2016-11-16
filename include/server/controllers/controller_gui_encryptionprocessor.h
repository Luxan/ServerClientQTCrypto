#pragma once

#include "../interfaces/interface_communication_controller.h"
#include "../modules/crypto_processor.h"
#include "../login_server/mainwindow.h"

/**
\class
\brief
*/
class Controller_GUI_EncryptionProcessor : public InterfaceCommunicationController
{
private:
    /**
    \see interface_communication_controller.h
    */
    void CheckModule1Events(void * module1, void * module2);

    /**
    \see interface_communication_controller.h
    */
    void CheckModule2Events(void * module1, void * module2);
public:
    void setMainWindowObj(MainWindow * module);
    void setEncryptionProcessorObj(EncryptionProcessor * module);
};

