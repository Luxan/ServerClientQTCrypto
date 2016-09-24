/**
\author Sergey Gorokh (ESEGORO)
*/
#include <string>

#include "../../../include/shared/messages/message.h"

 std::string Message::GetString()
 {
    return data;
 }

 bool MessageProcessable::isReadyToProcess()
 {
     return true;
 }
