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


void MessageTask::execute()
{
    ((MessageContext*)context)->m->process();
}

void CryptoTask::execute()
{

}
