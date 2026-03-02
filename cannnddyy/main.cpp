#include <QApplication>
#include <QWebEngineUrlScheme>
#include "browserwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWebEngineUrlScheme scheme("candy");
    scheme.setFlags(QWebEngineUrlScheme::LocalScheme |
                    QWebEngineUrlScheme::SecureScheme);
    QWebEngineUrlScheme::registerScheme(scheme);

    BrowserWindow window;
    window.show();

    return app.exec();
}
