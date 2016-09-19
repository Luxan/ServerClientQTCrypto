#pragma once

#include "../interfaces/interface_communication_controller.h"
#include "../modules/crypto_processor.h"
#include "../interfaces/interface_tcpchannel.h"

/**
\class
\brief
*/
class Controller_TCPChannel_DecryptionProcessor : public InterfaceCommunicationController
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
    void setTCPChannelObj(InterfaceTcpChannel * module);
    void setDecryptionProcessorObj(DecryptionProcessor * module);
};

