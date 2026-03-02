#include <QApplication>
#include <QWebEngineUrlScheme>
#include "browserwindow.h"

int main(int argc, char *argv[])
{
    QWebEngineUrlScheme scheme("candy");
    scheme.setFlags(QWebEngineUrlScheme::SecureScheme |
                    QWebEngineUrlScheme::LocalScheme |
                    QWebEngineUrlScheme::LocalAccessAllowed);
    QWebEngineUrlScheme::registerScheme(scheme);

    QApplication app(argc, argv);

    BrowserWindow window;
    window.show();

    return app.exec();
}
