#include <stdlib.h>
#include <time.h>

class TestBase : public QObject
{
    Q_OBJECT
private:
	time_t startTime;
protected:
	void showTimeElapsed()
	{
		qDebug("Test_Buffer Time Elapsed:" + QString(time(NULL) - startTime));
	}
private slots:
	virtual void constructor() = 0
	virtual void setup() = 0;
	virtual void tierDown() = 0;

	void setupTimeElapsed()
	{
		srand (time(NULL));
		startTime = time(NULL);
	}
		
};