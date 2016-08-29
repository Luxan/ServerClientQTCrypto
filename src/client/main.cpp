
#include <QApplication>
#include "../../include/client/mainwindow.h"
#include "../../include/client/loginwindow.h"
#include "../../include/shared/configuration.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow lw(globalConfiguration.minLoginCharacters, globalConfiguration.minPasswordCharacters);
    MainWindow mw;

    lw.show();
    mw.show();

    return a.exec();
}
