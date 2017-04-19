#pragma once

#include <QMainWindow>
#include <QTimer>
#include <mutex>
#include <queue>
#include <vector>

#include "impulse.h"
#include "interfaces/interface_communication_controller.h"

class Impulse;
class LoginWindow;

namespace Ui {
class MainWindow;
}

/**
\class
\brief
*/
class ChildController
{
private:
    bool db;
    bool ep;
    bool dp;
    bool mc;
    bool mp;
    bool tcpc;
public:
	ChildController();

    enum EnabledFlag
    {
//        Database,
//        EncryptionProcessor,
//        DecryptionProcessor,
//        MessageCollector,
//        MessageProcessor,
        TCP_Channel
    };

	bool isAllChildsEnabled()const;

	bool isAllChildsDisabled()const;

	void enableChild(ChildController::EnabledFlag child, bool enable);
};

/**
\class
\brief
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    std::queue<QString*> toAppendQueue;
    ChildController childController;
public:
    void setChildEnabled(ChildController::EnabledFlag child, bool enabled);

    bool isAllChildsEnabled()const;

    bool isAllChildsDisabled()const;

    void enableGui();


    static void putMSG(MainWindow *mw, const std::string &msg);

    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:

    void on_pushButton_clicked();
// --------------------------------- block from interfaceCommunication. it dont wanna compile if i inherit from it

    void checkAllEvents();

    void appendToEditor();

private:
    Ui::MainWindow *ui;
    std::mutex guiLock;

    // --------------------------------- block from interfaceCommunication. it dont wanna compile if i inherit from it

private:
    QTimer *eventTimer;
    QTimer *appendTimer;
    std::mutex lock_queue;
    std::vector<Impulse *> vImpulses;
    std::vector<InterfaceCommunicationController *> vControllers;
protected:

    void AddImpulseToQueue(Impulse *i);
public:

    Impulse *getNextImpulse(Impulse *i);

    Impulse *eraseImpulseAndGetNext(Impulse *i);

    void AddEventController(InterfaceCommunicationController *c);

    std::vector<Impulse *>::iterator beginIterator();

    std::vector<Impulse *>::iterator endIterator();

    std::vector<Impulse *>::iterator eraseImpulse(std::vector<Impulse *>::iterator &iter);

    void DestructorinterfaceCommunication();
};
