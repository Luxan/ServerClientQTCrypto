
#include <QApplication>
#include "../../include/client/mainwindow.h"
#include "../../include/client/loginwindow.h"
#include "../../include/client/tcpchannel.h"
#include "../../include/shared/configuration.h"
#include "../../include/shared/buffer.h"

void loadCertificate(QByteArray &cert)
{
    QFile file_cert("../../cert/CA/servercert.pem");
    if(file_cert.open(QIODevice::ReadOnly))
    {
        cert = file_cert.readAll();
        file_cert.close();
    }
    else
    {
        qDebug() << file_cert.errorString();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QByteArray cert;
    loadCertificate(cert);

    LoginWindow lw(globalConfiguration.minLoginCharacters, globalConfiguration.minPasswordCharacters);
    MainWindow mw;
    SslTcpChannel tcpChannel(globalConfiguration.serverIP, globalConfiguration.serverPort, cert);

    lw.setTCPChannel(&tcpChannel);
    mw.setTCPChannel(&tcpChannel);
    tcpChannel.setLoginWindow(&lw);
    tcpChannel.setMainWindow(&mw);

    lw.show();
    //mw.show();5

    return a.exec();
}
