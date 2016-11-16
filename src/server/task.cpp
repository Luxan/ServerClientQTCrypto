/**
\author Sergey Gorokh (ESEGORO)
*/
#include "../../include/server/task.h"

MessageContext::~MessageContext()
{
    delete m;
}

CryptoContext::~CryptoContext()
{
    delete m;
}

Task::Task(Context *_context):
    context(_context)
{}

Task::~Task()
{
    delete context;
}

Context * Task::getContext()
{
    return context;
}

void MessageTask::execute()
{
    //((MessageContext*)context)->m->process();
}

void MessageKeyAgreementTask::execute()
{
    MessageKeyAgreementContext* agentContext = (MessageKeyAgreementContext*)context;
    KeyAgreamentAgent * agent = agentContext->getAgent();
    MessageSessionDetailRequest * msg = dynamic_cast<MessageSessionDetailRequest*>(agentContext->extractMessage());
    if (msg != nullptr)
    {
        agent->verifyPGQ(msg->getP(), msg->getG(), msg->getQ());
        agent->prepareStaticAndEphemeralKeys();
        delete msg;
    }
}

Package * CryptoTask::createPackage(PackageBuffer * buf)
{
    PackageWrapper::ePackageType type;
    buf->fillBuffer((uint8_t*)&type, sizeof(type));
    Package * package = nullptr;
    switch (type) //change to if -> performance
    {
    case PackageWrapper::ePackageType::SessionDetailRequest:
        package = new PackageSessionDetailRequest(buf);
        break;
    case PackageWrapper::ePackageType::Ping:
        package = new PackagePing(buf);
        break;
    default:
        throw "Cannot create undefined type package!";
        break;
    }
    return package;
}

void CryptoTask::encrypt(PackageWrapper ** pw, Cipher * cipher)
{
    PackageWrapperDecoded * pwdec = (PackageWrapperDecoded*)*pw;
    PackageBuffer * input = pwdec->package->toPackageBuffer();
    PackageBuffer * output = new PackageBuffer(nullptr, 0);
    cipher->encrypt(input, output);
    delete input;
    delete pwdec;
    PackageWrapperEncoded * pwbuf = new PackageWrapperEncoded();
    pwbuf->buffer = output;
    pwbuf->sessionID = (*pw)->sessionID;
    *pw = pwbuf;
}

void CryptoTask::decrypt(PackageWrapper ** pw, Cipher * cipher)
{
    PackageWrapperEncoded * pwbuf = (PackageWrapperEncoded*)*pw;
    PackageWrapperDecoded * pwdec = new PackageWrapperDecoded();
    if (cipher == nullptr)
    {
        pwdec->package = createPackage(pwbuf->buffer);
        pwdec->sessionID = (*pw)->sessionID;
        delete pwbuf;
        *pw = pwdec;
    }
    else
    {
        PackageBuffer * input = pwbuf->buffer;
        PackageBuffer * output = new PackageBuffer(nullptr, 0);
        cipher->decrypt(input, output);
        delete pwbuf;
        pwdec->package = createPackage(output);
        pwdec->sessionID = (*pw)->sessionID;
        *pw = pwdec;
    }
}

void CryptoTask::execute()
{
    PackageWrapper * m = ((CryptoContext*)context)->extractWrapper();
    Cipher * cipher = ((CryptoContext*)context)->extractCipher();
    if (m == nullptr)
        return;

    if (cipher == nullptr && ((CryptoContext*)context)->isEncryption())
        return;

    if (((CryptoContext*)context)->isEncryption())
    {
        encrypt(&m, cipher);
    }
    else
    {
        decrypt(&m, cipher);
    }
    ((CryptoContext*)context)->setWrapper(m);
}
