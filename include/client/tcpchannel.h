#pragma once

#include <QObject>
#include <QTcpSocket>

class MainWindow;
class LoginWindow;

#include "../shared/packages/package.h"
#include "../client/mainwindow.h"
#include "../client/loginwindow.h"
#include "../shared/id_session.h"

class TCPChannel : public QObject
{
    Q_OBJECT

public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    explicit TCPChannel(const char *serverIP, const int serverPort);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ~TCPChannel();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool sendPackage(PackageWrapperDecoded *pw);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void setMainWindow(MainWindow * mw);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void setLoginWindow(LoginWindow * lw);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void startSession();
private slots:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void readTcpData();

private:
    QTcpSocket * _pSocket;
    MainWindow * mw;
    LoginWindow * lw;
    const char *serverIP;
    const int serverPort;
    SessionID sessionID;

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool connectToServer();

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool sendPackageMultyMessage(PackageWrapper::ePackageType type, PackageMultiPackage *mp);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool sendPackageDynamicMessage(PackageWrapper::ePackageType type, uint16_t size, uint8_t * buff, uint16_t sizeOfBuff);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool sendStrictSizePackage(PackageStrictSize * sp, uint8_t strictSize, PackageWrapper::ePackageType type);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool sendBuffer(uint8_t *buff, size_t size);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void processReceivedBuffers(std::list<PackageBuffer *> &list);

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    PackageWrapperDecoded *CreatePackage(PackageBuffer *buf);

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void logError(std::string error);

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void logInfo(std::string info);
};
