#pragma once

#include "../../include/server/controllers/controller_gui_tcpchannel.h"
#include "../../include/server/modules/epoll_ssl_channel.h"
#include "../../include/server/mainwindow.h"

#include "../../include/shared/user.h"
#include "../../include/shared/configuration.h"

namespace controllers
{
//controller gui - module
Controller_GUI_tcpChannel cgtc;

void initializeControllers();
}

namespace modules
{
EpollChannel *server;
MainWindow *w;

void zero();

void initializeMessageProcessor();

void initializeMessageCollector();

void initializeEpollTCPChannel(Buffer *certificate, Buffer *privateKey);

void initializeDataBase();

void initializeMainWindow();

void deinitializeAll();

}
