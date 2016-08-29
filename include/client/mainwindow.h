#pragma once

#include <memory>

#include <QTimer>
#include <QMainWindow>
#include "tcpchannel.h"
#include "../shared/id_client.h"
#include "../shared/user.h"

class TCPChannel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void setTCPChannel(TCPChannel * ch);

    void logError(std::string error);

    void logInfo(std::string info);

    void LoggedInAs(ClientID id);

private slots:
    void sendMyStatusToServer();
    void checkFriendsStatus();
private:
    Ui::MainWindow *ui;
    TCPChannel * tcpchannel;
    std::shared_ptr<User> activeUser;
    QTimer *timerPing;
    QTimer *timerCheckFriendsStatus;


    void getUserDetail(ClientID id);
};

