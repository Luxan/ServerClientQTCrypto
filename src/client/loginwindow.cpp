#include "ui_loginwindow.h"

#include <stdint.h>
#include <QDebug>

#include "../../include/client/tcpchannel.h"
#include "../../include/client/loginwindow.h"

#include "../../include/shared/packages/package_instant_replay.h"
#include "../../include/shared/packages/package_user_to_user.h"
#include "../../include/shared/packages/package_signal.h"
#include "../../include/shared/packages/package_update.h"
#include "../../include/shared/error_enum.h"
#include "../../include/client/clog.h"
#include "../../include/shared/buffer_spitter.h"

LoginWindow::LoginWindow(const int minLoginCharacters, const int minPasswordCharacters, QWidget *parent) :
    QMainWindow(parent),
    minLoginCharacters(minLoginCharacters),
    minPasswordCharacters(minPasswordCharacters),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    ui->passwordEdit->setEchoMode(QLineEdit::Password);
    ui->passwordEdit->setInputMethodHints(Qt::ImhHiddenText | Qt::ImhNoPredictiveText | Qt::ImhNoAutoUppercase);
}

void LoginWindow::setTCPChannel(TCPChannel * ch)
{
    tcpchannel = ch;

    try
    {
        tcpchannel->startSession();
    }
    catch (eError e)
    {
        switch (e)
        {
        case eError::ServerIsAnaviable:
            logError("Server is unaviable!");
            break;
        default:
            logError("Undefined error!");
            break;
        }
    }
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::logError(std::string error)
{
    ui->statusLabel->setStyleSheet("QLabel { color : red;}");
    ui->statusLabel->setText(QString(error.c_str()));
}

void LoginWindow::logInfo(std::string info)
{
    ui->statusLabel->setStyleSheet("QLabel { color : black;}");
    ui->statusLabel->setText(QString(info.c_str()));
}

void LoginWindow::on_loginButton_clicked()
{
    QString login = ui->loginEdit->text();
    QString password = ui->passwordEdit->text();

    if (login.length() < minLoginCharacters)
    {
        ui->statusLabel->setText(QString("Login must have at least ") +
                                 QString::number(minLoginCharacters) +
                                 QString(" characters!"));
        ui->statusLabel->setStyleSheet("QLabel { color : red;}");
        return;
    }
    if (password.length() < minPasswordCharacters)
    {
        ui->statusLabel->setText(QString("Password must have at least ") +
                                 QString::number(minPasswordCharacters) +
                                 QString(" characters!"));
        ui->statusLabel->setStyleSheet("QLabel { color : red;}");
        return;
    }

    ui->statusLabel->setText(QString("Signing in.."));
    ui->statusLabel->setStyleSheet("QLabel { color : black;}");

    PackageWrapper wr;
    try
    {
        //wr.package = new PackageRequestLogin(login.toStdString(), password.toStdString());
        //wr.type = PackageWrapper::ePackageType::RequestLogin;
        //tcpchannel->sendPackage(&wr);
        //delete wr.package;
    }
    catch (eError e)
    {
        switch (e)
        {
        case eError::ServerIsAnaviable:
            logError("Server is unaviable!");
            break;
        case eError::Undefined:
        case eError::WrongLoginOrPassword:
            logError("Undefined error!");
            break;
        }
        delete wr.package;
    }
}
//#include "moc_mainwindow.cpp"
