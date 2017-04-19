#pragma once

#include <memory>

#include "../shared/user.h"
#include "../shared/package_buffer.h"

class RemoteClient
{
    std::shared_ptr<User> u;
    bool isConn;
public:
    PackageBuffer *incompletePackageBuffer;
    BUFF_SIZE incompletePackageFullLength;

    RemoteClient()
    {
        isConn = false;
        incompletePackageBuffer = new PackageBuffer(nullptr, 0);
        incompletePackageFullLength = 0;
    }

    ~RemoteClient()
    {
        delete incompletePackageBuffer;
    }

    void connectUser(std::shared_ptr<User> _u)
    {
        u = _u;
        isConn = true;
    }

    void releaseUser()
    {
        isConn = false;
        delete incompletePackageBuffer;
        incompletePackageBuffer = new PackageBuffer(nullptr, 0);
        incompletePackageFullLength = 0;
    }

    bool isConnected()const
    {
        return isConn;
    }

    std::shared_ptr<User> getUser()
    {
        return u;
    }
};

