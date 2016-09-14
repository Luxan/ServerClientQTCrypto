#pragma once

/*!
\struct  GlobalConfiguration
\brief const object of a struct for global configuration. Will be initialized from file in the future.
*/
const struct GlobalConfiguration
{
    const int minPasswordCharacters = 8; /*! define min size of password characters used for logging in */
    const int minLoginCharacters = 4; /*! define min size of login characters used for logging in  */
    const int serverPort = 5555; /*! define server port to listen/send to*/
    const char *serverIP = "127.0.0.1"; /*! define server IP to listen/send to */
}globalConfiguration; /*! const object of GlobalConfiguration to initialize all parameters in communication system client-server. */
