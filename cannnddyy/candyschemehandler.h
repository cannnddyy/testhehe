#ifndef CANDYSCHEMEHANDLER_H
#define CANDYSCHEMEHANDLER_H

#include <QtWebEngineCore/QWebEngineUrlSchemeHandler>
#include <QObject>

class CandySchemeHandler :
public QWebEngineUrlSchemeHandler
{
public:
    CandySchemeHandler(QObject *parent = nullptr);
    void requestStarted(QWebEngineUrlRequestJob *job) override;
};

#endif
