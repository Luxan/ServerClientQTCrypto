#pragma once

#include <QMainWindow>
#include "tcpchannel.h"

class SslTcpChannel;

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(const int minLoginCharacters, const int minPasswordCharacters, QWidget *parent = 0);
    virtual ~LoginWindow();
    void setTCPChannel(SslTcpChannel * ch);
    void logError(std::string error);
    void logInfo(std::string info);

private slots:
    void on_loginButton_clicked();

private:
    const int minLoginCharacters;
    const int minPasswordCharacters;
    SslTcpChannel * tcpchannel;
    Ui::LoginWindow *ui;
};
