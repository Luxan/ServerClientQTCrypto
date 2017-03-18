#pragma once

#include <QObject>
#include <QtNetwork>

class MainWindow;
class LoginWindow;

#include "../shared/packages/package.h"
#include "../client/mainwindow.h"
#include "../client/loginwindow.h"
#include "../shared/id_session.h"

class SslTcpChannel : public QTcpServer
{
    Q_OBJECT

public:
    explicit SslTcpChannel(const char *serverIP, const int serverPort, QByteArray& cert);
    virtual ~SslTcpChannel();
    bool sendPackage(PackageWrapperDecoded *pw);
    void setMainWindow(MainWindow * mw);
    void setLoginWindow(LoginWindow * lw);
    void startSession();

private slots:
    void readTcpData();
    void encrypted();
    void sslError( QList<QSslError> errors );

private:
    //ssl
    QSslSocket * _pSocket;
    QSslCertificate ssl_cert;
    //modules
    MainWindow * mw;
    LoginWindow * lw;
    //host
    const char *serverIP;
    const int serverPort;
    //connection
    SessionID sessionID;
    bool connectToServer();
    bool disconnectFronServer();
    bool sendPackageMultyMessage(PackageWrapper::ePackageType type, PackageMultiPackage *mp);
    bool sendPackageDynamicMessage(PackageWrapper::ePackageType type, uint16_t size, uint8_t * buff, uint16_t sizeOfBuff);
    bool sendStrictSizePackage(PackageStrictSize * sp, uint8_t strictSize, PackageWrapper::ePackageType type);
    bool sendBuffer(uint8_t *buff, size_t size);
    void processReceivedBuffers(std::list<PackageBuffer *> &list);
    PackageWrapperDecoded *CreatePackage(PackageBuffer *buf);
    void logError(std::string error);
    void logInfo(std::string info);
};
