#include "candyschemehandler.h"
#include <QFile>

CandySchemeHandler::CandySchemeHandler(QObject *parent)
    : QWebEngineUrlSchemeHandler(parent)
{
}

void CandySchemeHandler::requestStarted(
    QWebEngineUrlRequestJob *job)
{
    QString path = job->requestUrl().path();

    QString filePath = ":/pages" + path + ".html";

    QFile *file = new QFile(filePath);

    if(file->open(QIODevice::ReadOnly))
        job->reply("text/html", file);
    else
    {
        QByteArray *data =
        new QByteArray("<h1>Page Not Found</h1>");
        job->reply("text/html", data);
    }
}
