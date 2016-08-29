#pragma once

struct GlobalConfiguration
{
    const int minPasswordCharacters = 8;
    const int minLoginCharacters = 4;
    const int serverPort = 5555;
    const char *serverIP = "127.0.0.1";
}globalConfiguration;
