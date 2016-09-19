
#include "../../forms/server/ui_mainwindow.h"
#include "../../include/server/mainwindow.h"
#include "../../include/server/impulse.h"

ChildController::ChildController()
{
    db = false;
    ep = false;
    dp = false;
	mc = false;
	mp = false;
	tcpc = false;
}

bool ChildController::isAllChildsEnabled()const
{
    return db && ep && dp && mc && mp && tcpc;
}

bool ChildController::isAllChildsDisabled()const
{
    return (!db) && (!ep) && (!dp) && (!mc) && (!mp) && (!tcpc);
}

void ChildController::enableChild(ChildController::EnabledFlag child, bool enable)
{
	switch (child)
	{
    case Database:
        db = enable;
        break;
    case EncryptionProcessor:
        ep = enable;
        break;
    case DecryptionProcessor:
        dp = enable;
        break;
	case MessageCollector:
		mc = enable;
		break;
	case MessageProcessor:
		mp = enable;
		break;
	case TCP_Channel:
		tcpc = enable;
		break;
	}
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    eventTimer = new QTimer(this);
    connect(eventTimer, SIGNAL(timeout()), this, SLOT(checkAllEvents()));
    eventTimer->start(1000);


    appendTimer = new QTimer(this);
    connect(appendTimer, SIGNAL(timeout()), this, SLOT(appendToEditor()));
    appendTimer->start(100);
}

MainWindow::~MainWindow()
{
    DestructorinterfaceCommunication();
    delete ui;
    delete eventTimer;
    delete appendTimer;
}

void MainWindow::setChildEnabled(ChildController::EnabledFlag child, bool enabled)
{
    childController.enableChild(child, enabled);
}

bool MainWindow::isAllChildsEnabled()const
{
    return childController.isAllChildsEnabled();
}

bool MainWindow::isAllChildsDisabled()const
{
    return childController.isAllChildsDisabled();
}

void MainWindow::enableGui()
{
    if (ui->pushButton->text() == "Start")
    {
        ui->pushButton->setText("Stop");
        ui->pushButton->setDisabled(false);
    }
    else
    {
        ui->pushButton->setText("Start");
        ui->pushButton->setDisabled(false);
    }
}

void MainWindow::putMSG(MainWindow *mw, const std::string &msg)
{
    std::lock_guard<std::mutex> guard(mw->guiLock);
    QString *str = new QString(__DATE__);
    *str += " : ";
    *str += msg.c_str();

    mw->toAppendQueue.push(str);
}

void MainWindow::appendToEditor()
{
    while (!toAppendQueue.empty())
    {
        guiLock.lock();
        QString * str = toAppendQueue.front();
        toAppendQueue.pop();
        guiLock.unlock();

        ui->textBrowser->append(*str);

        delete str;
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->pushButton->text() == "Start")
    {
//        ui->pushButton->setText("Stop");

        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestStartDatabase));
        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestStartDecryptionProcessor));
        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestStartEncryptionProcessor));
        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestStartTcpChannel));
        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestStartMessageCollector));
        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestStartMessageProcessor));

        ui->pushButton->setDisabled(true);
    }
    else
    {
//        ui->pushButton->setText("Start");


        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestSleepDatabase));
        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestSleepEncryptionProcessor));
        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestSleepDecryptionProcessor));
        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestSleepTcpChannel));
        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestSleepMessageCollector));
        AddImpulseToQueue(new ImpulseSignal(eSystemEvent::RequestSleepMessageProcessor));

        ui->pushButton->setDisabled(true);
    }
}

//------------ block from interfaceCommunication

void MainWindow::AddImpulseToQueue(Impulse *i)
{
    std::lock_guard<std::mutex> guard(lock_queue);
    vImpulses.push_back(i);
}

void MainWindow::checkAllEvents()
{
    for (InterfaceCommunicationController *c : vControllers)
    {
        c->CheckEvents(this);
    }
}

Impulse * MainWindow::getNextImpulse(Impulse *i)
{
    std::lock_guard<std::mutex> guard(lock_queue);
    if (i == nullptr)
    {
        if (vImpulses.empty())
            return nullptr;

        return *(vImpulses.begin());
    }

    i->setProcessingState(false);

    bool found = false;

    for (std::vector<Impulse *>::iterator it = vImpulses.begin(); it != vImpulses.end(); it++)
    {
        if (!found)
        {
            if (i == *it)
            {
                found = true;
            }

            continue;
        }

        if ((*it)->isInProcessingState())
        {
            continue;
        }

        (*it)->setProcessingState(true);
        return *it;
    }

    return nullptr;
}

Impulse *MainWindow::eraseImpulseAndGetNext(Impulse *i)
{
    std::lock_guard<std::mutex> guard(lock_queue);

    if (i == nullptr)
    {
        return nullptr;
    }

    bool erase = true;

    i->setProcessingState(false);

    for (std::vector<Impulse *>::iterator it = vImpulses.begin(); it != vImpulses.end(); it++)
    {
        if (erase)
        {
            if (i == *it)
            {
                it = vImpulses.erase(it);
                erase = false;
            }
            else
            {
                continue;
            }
        }

        if (it == vImpulses.end())
        {
            return nullptr;
        }

        Impulse * i = *it;

        if (i == nullptr)
        {
            break;
        }

        if (i->isInProcessingState())
        {
            continue;
        }


        (*it)->setProcessingState(true);
        return *it;
    }

    return nullptr;
}

void MainWindow::AddEventController(InterfaceCommunicationController *c)
{
    vControllers.push_back(c);
}

void MainWindow::DestructorinterfaceCommunication()
{
    std::lock_guard<std::mutex> guard(lock_queue);

    for (Impulse *i : vImpulses)
    {
        delete i;
    }
}
