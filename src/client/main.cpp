
#include <QApplication>
#include "../../include/client/mainwindow.h"
#include "../../include/client/loginwindow.h"
#include "../../include/client/tcpchannel.h"
#include "../../include/shared/configuration.h"
#include "../../include/shared/buffer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow lw(globalConfiguration.minLoginCharacters, globalConfiguration.minPasswordCharacters);
    MainWindow mw;
    TCPChannel tcpChannel(globalConfiguration.serverIP, globalConfiguration.serverPort);

    lw.setTCPChannel(&tcpChannel);
    mw.setTCPChannel(&tcpChannel);
    tcpChannel.setLoginWindow(&lw);
    tcpChannel.setMainWindow(&mw);

    lw.show();
    //mw.show();

    return a.exec();
}
