#include <QApplication>
#include "browserwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    BrowserWindow window;
    window.resize(1200, 800);
    window.show();

    return app.exec();
}
