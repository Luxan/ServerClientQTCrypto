#include "../../include/shared/error_enum.h"
#include "../../include/shared/packages/package_instant_replay.h"
#include "../../include/shared/packages/package_user_to_user.h"
#include "../../include/shared/packages/package_signal.h"
#include "../../include/shared/packages/package_update.h"
#include "../../include/client/clog.h"
#include "../../include/shared/buffer_spitter.h"

#include "../../include/client/loginwindow.h"
#include "../../include/client/mainwindow.h"
#include "../../include/client/tcpchannel.h"

TCPChannel::TCPChannel(const char *serverIP, const int serverPort, CertificateAuthority * ca):
    serverIP(serverIP),
    serverPort(serverPort),
    ca(ca)
{}

TCPChannel::~TCPChannel()
{
    delete _pSocket;
}

bool TCPChannel::connectToServer()
{
    _pSocket = new QTcpSocket();
    connect(_pSocket, SIGNAL(readyRead()), SLOT(readTcpData()));

    _pSocket->connectToHost(serverIP, serverPort);
    return _pSocket->waitForConnected();
}

bool TCPChannel::sendPackageMultyMessage(PackageWrapper::ePackageType type, PackageMultiPackage *mp)
{
    uint8_t size = sizeof(type) + sizeof(mp->multiPackSize) +
                   sizeof(mp->multiPackCurrentSize) + mp->buff->getLength();

    if (!sendBuffer(&size, sizeof(size)))
        return false;

    if (!sendBuffer((uint8_t *)&type, sizeof(type)))
        return false;

    if (!sendBuffer(&mp->multiPackSize, sizeof(mp->multiPackSize)))
        return false;

    if (!sendBuffer(&mp->multiPackCurrentSize, sizeof(mp->multiPackCurrentSize)))
        return false;

    if (!sendBuffer(mp->buff->getPointerToBuffer(), mp->buff->getLength()))
        return false;

    return true;
}

bool TCPChannel::sendPackageDynamicMessage(PackageWrapper::ePackageType type, uint16_t size, uint8_t * buff, uint16_t sizeOfBuff)
{
    if (!sendBuffer(&size, sizeof(size)))
        return false;

    if (!sendBuffer((uint8_t *)&type, sizeof(type)))
        return false;

    if (!sendBuffer(buff, sizeOfBuff))
        return false;

    return true;
}

bool TCPChannel::sendStrictSizePackage(PackageStrictSize * sp, uint8_t strictSize, PackageWrapper::ePackageType type)
{
    uint8_t size = (uint8_t) strictSize + sizeof(type);
    if (!sendBuffer((uint8_t *)&size, sizeof(size)))
        return false;

    if (!sendBuffer((uint8_t *)&type, sizeof(type)))
        return false;

    if (!sendBuffer((uint8_t *)sp, strictSize))
        return false;

    return true;
}

bool TCPChannel::sendBuffer(uint8_t *buff, size_t size)
{
    Quint8_tArray dataWrite((const char *)buff, size);

    //for (size_t i = 0; i < size; i++)
    //{
    //    qDebug() << "0x" << std::hex << *(buff + i) << '\n' ;
    //}

    _pSocket->write(dataWrite);
    return _pSocket->waitForuint8_tsWritten();
}

bool TCPChannel::sendPackage(PackageWrapper *pw)
{
    //Quint8_tArray data; // <-- fill with data

    if (!connectToServer())
    {
        throw (eError::ServerIsAnaviable);
    }
    PackageRequestLogin *rl;
    PackageRequestAutocomplete *ra;
    uint16_t size;
    uint8_t strictSize;
    switch (pw->type)
    {
    case PackageWrapper::ePackageType::SessionDetailRequest:
        size = sizeof(pw->type);

        if (!sendBuffer(&size, sizeof(size)))
            return false;

        if (!sendBuffer((uint8_t *)&pw->type, sizeof(pw->type)))
            return false;

    //dynamic size packages
    case PackageWrapper::ePackageType::RequestLogin:
        rl = (PackageRequestLogin *)pw->package;

        size = (uint8_t)(rl->realSize() + sizeof(pw->type));

        if (!sendBuffer(&size, sizeof(size)))
            return false;

        if (!sendBuffer((uint8_t *)&pw->type, sizeof(pw->type)))
            return false;

        if (!sendBuffer(&rl->loginSize, sizeof(rl->loginSize)))
            return false;
        if (!sendBuffer(rl->login, rl->loginSize))
            return false;
        if (!sendBuffer(&rl->passwordSize, sizeof(rl->passwordSize)))
            return false;
        if (!sendBuffer(rl->password, rl->passwordSize))
            return false;

        break;
    case PackageWrapper::ePackageType::RequestAutocomplete:
        ra = (PackageRequestAutocomplete *)pw->package;

        size = (uint8_t)(ra->realSize() + sizeof(pw->type));

        if (!sendBuffer(&size, sizeof(size)))
            return false;

        if (!sendBuffer((uint8_t *)&pw->type, sizeof(pw->type)))
            return false;

        size = ra->buff->getLength();
        if (!sendBuffer(&size, sizeof(size)))
            return false;
        if (!sendBuffer(ra->buff->getPointerToBuffer(), ra->buff->getLength()))
            return false;

    case PackageWrapper::ePackageType::UserMessage:
    case PackageWrapper::ePackageType::UserNotification:
        if (!sendPackageMultyMessage(pw->type, (PackageMultiPackage *)pw->package))
            return false;
        break;
    //strict size packages
    case PackageWrapper::ePackageType::SetUserInBlackList:
        strictSize = ((PackageSetUserInBlackList *) pw->package)->strictSize();
        if (!sendStrictSizePackage((PackageStrictSize *) pw->package, strictSize, pw->type))
            return false;
        break;
    case PackageWrapper::ePackageType::RequestLeavingRoom:
        strictSize = ((PackageRequestLeavingRoom *) pw->package)->strictSize();
        if (!sendStrictSizePackage((PackageStrictSize *) pw->package, strictSize, pw->type))
            return false;
        break;
    case PackageWrapper::ePackageType::RequestMessageHistory:
        strictSize = ((PackageRequestMessageHistory *) pw->package)->strictSize();
        if (!sendStrictSizePackage((PackageStrictSize *) pw->package, strictSize, pw->type))
            return false;
        break;
    case PackageWrapper::ePackageType::RequestPublicKey:
        strictSize = ((PackageRequestPublicKey *) pw->package)->strictSize();
        if (!sendStrictSizePackage((PackageStrictSize *) pw->package, strictSize, pw->type))
            return false;
        break;
    case PackageWrapper::ePackageType::RequestJoinRoom:
        strictSize = ((PackageRequestJoinRoom *) pw->package)->strictSize();
        if (!sendStrictSizePackage((PackageStrictSize *) pw->package, strictSize, pw->type))
            return false;
        break;
    case PackageWrapper::ePackageType::UnFriendUser:
        strictSize = ((PackageUnFriendUser *) pw->package)->strictSize();
        if (!sendStrictSizePackage((PackageStrictSize *) pw->package, strictSize, pw->type))
            return false;
        break;
    case PackageWrapper::ePackageType::Ping:
        strictSize = ((PackagePing *) pw->package)->strictSize();
        if (!sendStrictSizePackage((PackageStrictSize *) pw->package, strictSize, pw->type))
            return false;
        break;
    case PackageWrapper::ePackageType::Error:
        strictSize = ((PackageError *) pw->package)->strictSize();
        if (!sendStrictSizePackage((PackageStrictSize *) pw->package, strictSize, pw->type))
            return false;
        break;
    case PackageWrapper::ePackageType::RequestUserDetails:
        strictSize = ((PackageRequestUserDetails *) pw->package)->strictSize();
        if (!sendStrictSizePackage((PackageStrictSize *) pw->package, strictSize, pw->type))
            return false;
        break;
    // packages that should never be sent by Client
    case PackageWrapper::ePackageType::ResponseLogin:
    case PackageWrapper::ePackageType::ResponsePublicKey:
    case PackageWrapper::ePackageType::ResponseAutocomplete:
    case PackageWrapper::ePackageType::UpdateBlackList:
    case PackageWrapper::ePackageType::UpdateFriendStatus:
    case PackageWrapper::ePackageType::UpdateRoomPresence:
    case PackageWrapper::ePackageType::UpdateBlackListPresence:
    case PackageWrapper::ePackageType::ResponseUserDetails:
        CLog::logError() << "Incorrect package to send!";
        break;
    }
    return true;
}

void TCPChannel::readTcpData()
{
    Quint8_tArray data = _pSocket->readAll();

    bufferSpitter b((uint8_t *)data.data(), data.length());
    std::list<PackageBuffer *> list;
    static PackageBuffer incompletePackageBuffer(nullptr, 0);
    static size_t incompletePackageFullLength = 0;
    b.splitBufferIntoList(list, &incompletePackageBuffer, incompletePackageFullLength);
    processReceivedBuffers(list);
}

void TCPChannel::processReceivedBuffers(std::list<PackageBuffer *> &list)
{
    for (PackageBuffer *packageBuff : list)
    {
        uint8_t test;
        for (size_t i = 0; i < packageBuff->getLength(); i++)
        {
            test = *(packageBuff->getPointerToBuffer() + i);
            qDebug() << test;
        }
        PackageWrapper *pw = CreatePackage(packageBuff);

        if (pw == nullptr)
            return;

        if (pw->type == PackageWrapper::ePackageType::SessionDetailResponse)
        {
            if (!ca->authorize(((PackageSessionDetailResponse*)pw->package)->certificate))
                logError("Certificate Authority cannot authorize server's certificate!");
            continue;
        }
        if (pw->type == PackageWrapper::ePackageType::)
        {
            //logInfo("Signed in Successfully!");

            mw->LoggedInAs(((PackageResponseLogin*)pw->package)->id);

            delete pw->package;
            return;
        }
        if (pw->type == PackageWrapper::ePackageType::Error)
        {
            uint8_t error = ((PackageError*)pw->package)->errorNumber;
            switch (error)
            {
            case eError::WrongLoginOrPassword:
                logError("Wrong Login or Password!");
                break;
            case eError::ServerIsAnaviable:
                logError("Server is unaviable!");
                break;
            default:
                logError("Undefined error!");
                break;
            }

            delete pw->package;
            return;
        }
    }
}

void TCPChannel::logError(std::string error)
{
    if (!this->mw->isHidden())
        this->mw->logError(error);
    if (!this->lw->isHidden())
        this->lw->logError(error);

    CLog::logError() << error;
}

void TCPChannel::logInfo(std::string info)
{
    if (!mw->isHidden())
        mw->logInfo(info);
    if (!lw->isHidden())
        lw->logInfo(info);

    CLog::logInfo() << info;
}
void TCPChannel::setMainWindow(MainWindow * mw)
{
    this->mw = mw;
}

void TCPChannel::setLoginWindow(LoginWindow * lw)
{
    this->lw = lw;
}

PackageWrapper *TCPChannel::CreatePackage(PackageBuffer *buf)
{
    if (buf == nullptr)
        return nullptr;

    PackageWrapper *pw = new PackageWrapper();
    try
    {
        buf->fillBuffer((uint8_t *)&pw->type, sizeof(pw->type));

        switch (pw->type)//change to if -> performance
        {
        case PackageWrapper::ePackageType::SessionDetailRequest:
            throw ("why would server send SessionDetailRequest to client?!");
            break;
        case PackageWrapper::ePackageType::SessionDetailResponse:
            pw->package = new PackageSessionDetailResponse(buf);
            delete buf;
            break;



        case PackageWrapper::ePackageType::RequestUserDetails:
            throw ("why would server send RequestUserDetails to client?!");
            break;
        case PackageWrapper::ePackageType::RequestAutocomplete:
            throw ("why would server send RequestAutocomplete to client?!");
            break;
        case PackageWrapper::ePackageType::RequestLeavingRoom:
            throw ("why would server send RequestLeavingRoom to client?!");
            break;
        case PackageWrapper::ePackageType::RequestPublicKey:
            throw ("why would server send RequestPublicKey to client?!");
            break;
        case PackageWrapper::ePackageType::RequestMessageHistory:
            throw ("why would server send RequestMessageHistory to client?!");
            break;
        case PackageWrapper::ePackageType::RequestJoinRoom:
            throw ("why would server send RequestJoinRoom to client?!");
            break;
        case PackageWrapper::ePackageType::RequestLogin:
            throw ("why would server send RequestLogin to client?!");
            break;
        case PackageWrapper::ePackageType::SetUserInBlackList:
            throw ("why would server send SetUserInBlackList to client?!");
            break;
        case PackageWrapper::ePackageType::UnFriendUser:
            throw ("why would server send UnFriendUser to client?!");
            break;
        case PackageWrapper::ePackageType::ResponseUserDetails:
            pw->package = new PackageResponseUserDetails(buf);
            break;
        case PackageWrapper::ePackageType::ResponseAutocomplete:
            pw->package = new PackageResponseAutocomplete(buf);
            break;
        case PackageWrapper::ePackageType::UpdateBlackList:
            pw->package = new PackageUpdateBlackList(buf);
            break;
        case PackageWrapper::ePackageType::UpdateFriendStatus:
            pw->package = new PackageUpdateFriendStatus(buf);
            break;
        case PackageWrapper::ePackageType::UpdateRoomPresence:
            pw->package = new PackageUpdateRoomPresence(buf);
            break;
        case PackageWrapper::ePackageType::UpdateBlackListPresence:
            pw->package = new PackageUpdateBlackListPresence(buf);
            break;
        case PackageWrapper::ePackageType::UserMessage:
            pw->package = new PackageUserMessage(buf);
            break;
        case PackageWrapper::ePackageType::UserNotification:
            pw->package = new PackageUserNotification(buf);
            break;
        //strict size
        case PackageWrapper::ePackageType::ResponsePublicKey:
            pw->package = new PackageResponsePublicKey();
            memcpy(pw->package, buf->getPointerToBuffer(), sizeof(PackageResponsePublicKey));
            delete buf;
            break;
        case PackageWrapper::ePackageType::ResponseLogin:
            pw->package = new PackageResponseLogin();
            memcpy(pw->package, buf->getPointerToBuffer(), sizeof(PackageResponseLogin));
            delete buf;
            break;
        case PackageWrapper::ePackageType::Ping:
            pw->package = new PackagePing();
            memcpy(pw->package, buf->getPointerToBuffer(), sizeof(PackagePing));
            delete buf;
            break;
        case PackageWrapper::ePackageType::Error:
            pw->package = new PackageError();
            memcpy(pw->package, buf->getPointerToBuffer(), sizeof(PackageError));
            delete buf;
            break;
        default:
            throw ("Undefined package type!");
        }
        return pw;
    }
    catch (std::string error)
    {
        logError(error);
        delete buf;
        delete pw;

        return nullptr;
    }
}
