#pragma once
#include <QObject>
#include <stdlib.h>
#include <chrono>

class TestBase : public QObject
{
    Q_OBJECT
private:
    std::chrono::time_point<std::chrono::system_clock> startTime;
protected:
    void setupTimeElapsed()
    {
        srand (time(NULL));
        startTime = std::chrono::system_clock::now();
    }
	void showTimeElapsed()
	{
        std::chrono::duration<double> diff = std::chrono::system_clock::now() - startTime;
        std::string q("Time Elapsed:" + std::to_string(diff.count()));
        qDebug(q.c_str());
	}

    virtual void setup() = 0;
    virtual void tearDown() = 0;
    virtual void constructor() = 0;
public:
    TestBase()
    {
        setupTimeElapsed();
    }

    ~TestBase()
    {
        showTimeElapsed();
    }
};
