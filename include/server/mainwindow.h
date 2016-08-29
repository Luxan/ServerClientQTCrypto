#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <mutex>
#include <queue>
#include <vector>

#include "../server/impulse.h"
#include "../server/interface_communication_controller.h"

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
    bool mc;
    bool mp;
    bool tcpc;
public:
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
	ChildController();

    enum EnabledFlag
    {
        MessageCollector,
        MessageProcessor,
        TCP_Channel
    };
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
	bool isAllChildsEnabled()const;
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
	bool isAllChildsDisabled()const;
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
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
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void setChildEnabled(ChildController::EnabledFlag child, bool enabled);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool isAllChildsEnabled()const;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    bool isAllChildsDisabled()const;
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void enableGui();

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    static void putMSG(MainWindow *mw, const std::string &msg);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    explicit MainWindow(QWidget *parent = 0);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    ~MainWindow();

private slots:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void on_pushButton_clicked();
// --------------------------------- block from interfaceCommunication. it dont wanna compile if i inherit from it
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void checkAllEvents();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
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
    std::vector<interfaceCommunicationController *> vControllers;
protected:
    /**
    \threadsafe is thread safe!
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void AddImpulseToQueue(Impulse *i);
public:
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    Impulse *getNextImpulse(Impulse *i);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    Impulse *eraseImpulseAndGetNext(Impulse *i);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void AddEventController(interfaceCommunicationController *c);
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    std::vector<Impulse *>::iterator beginIterator();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    std::vector<Impulse *>::iterator endIterator();

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    std::vector<Impulse *>::iterator eraseImpulse(std::vector<Impulse *>::iterator &iter);

    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void DestructorinterfaceCommunication();
};

#endif // MAINWINDOW_H
