#include "ui_mainwindow.h"
#include <QTimer>
#include "../../include/client/mainwindow.h"
#include "../../include/shared/package_instant_replay.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTCPChannel(TCPChannel *ch)
{
    this->tcpchannel = ch;
}

void MainWindow::logError(std::string error)
{
    ui->statusLabel->setStyleSheet("QLabel { color : red;}");
    ui->statusLabel->setText(QString(error.c_str()));
}

void MainWindow::logInfo(std::string info)
{
    ui->statusLabel->setStyleSheet("QLabel { color : black;}");
    ui->statusLabel->setText(QString(info.c_str()));
}

void MainWindow::LoggedInAs(ClientID id)
{
    timerPing = new QTimer(this);
    connect(timerPing, SIGNAL(timeout()), this, SLOT(sendMyStatusToServer()));
    timerPing->start(1000);


    timerCheckFriendsStatus = new QTimer(this);
    connect(timerCheckFriendsStatus, SIGNAL(timeout()), this, SLOT(checkFriendsStatus()));
    timerCheckFriendsStatus->start(1000);


}

void MainWindow::getUserDetail(ClientID id)
{

    PackageWrapper * pw = new PackageWrapper();

    pw->type = PackageWrapper::ePackageType::Ping;
    pw->package = new PackagePing();
    tcpchannel->sendPackage(pw);
    delete pw->package;
    delete pw;
}

void MainWindow::checkFriendsStatus()
{
    /*
    std::for_each (this->)
    PackageWrapper * pw = new PackageWrapper();
    pw->type = PackageWrapper::ePackageType::RequestUserDetails;
    pw->package = new PackageRequestUserDetails(activeUser->getStatus());

    tcpchannel->sendPackage(pw);

    delete pw->package;
    delete pw;
    */
}

void MainWindow::sendMyStatusToServer()
{
    PackageWrapper * pw = new PackageWrapper();
    pw->type = PackageWrapper::ePackageType::Ping;
    pw->package = new PackagePing(activeUser->getStatus());

    tcpchannel->sendPackage(pw);

    delete pw->package;
    delete pw;
}
