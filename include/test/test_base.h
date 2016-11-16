#pragma once
#include <QObject>
#include <QFile>
#include <QTreeWidget>
#include <stdlib.h>
#include <chrono>

class TestBase : public QObject
{
    Q_OBJECT
private:
    //static StatsCollector sc;

    std::string name;
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
        std::string q(name + " \t Elapsed:" + std::to_string(diff.count()));
        qDebug(q.c_str());
	}

    virtual void setup() = 0;
    virtual void tearDown() = 0;
    virtual void constructor() = 0;
public:
    TestBase(std::string name):
        name(name)
    {
        setupTimeElapsed();
    }

    virtual ~TestBase()
    {
        showTimeElapsed();
    }
};

#define CLASSTEST(Class, Base) \
public: \
Class(std::string name):\
    Base(name)\
{}
