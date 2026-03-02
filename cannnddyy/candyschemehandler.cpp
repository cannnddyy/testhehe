#include "candyschemehandler.h"
#include <QFile>

void CandySchemeHandler::requestStarted(QWebEngineUrlRequestJob *job)
{
    QString path = job->requestUrl().path();
    QString filePath;

    if (path == "/about")
        filePath = ":/pages/about.html";
    else if (path == "/history")
        filePath = ":/pages/history.html";
    else if (path == "/settings")
        filePath = ":/pages/settings.html";
    else
        filePath = ":/pages/about.html";

    QFile *file = new QFile(filePath);

    if (!file->open(QIODevice::ReadOnly)) {
        job->fail(QWebEngineUrlRequestJob::UrlNotFound);
        return;
    }

    job->reply("text/html", file);
}
