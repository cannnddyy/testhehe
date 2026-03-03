#include <QApplication>
#include "browserwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    BrowserWindow w;
    w.showMaximized();

    return a.exec();
}
