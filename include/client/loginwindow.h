#pragma once

#include <QMainWindow>
#include "tcpchannel.h"

class TCPChannel;

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
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
    explicit LoginWindow(const int minLoginCharacters, const int minPasswordCharacters, QWidget *parent = 0);
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    virtual ~LoginWindow();
    /**
    \param
    \return
    \throw
    \brief
    \pre
    \post
    */
    void setTCPChannel(TCPChannel * ch);
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

private slots:
	/**
	\param
	\return
	\throw
	\brief
	\pre
	\post
	*/
    void on_loginButton_clicked();

private:
    const int minLoginCharacters;
    const int minPasswordCharacters;
    TCPChannel * tcpchannel;
    Ui::LoginWindow *ui;
};
