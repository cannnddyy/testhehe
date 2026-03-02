#pragma once

#include <QWebEngineUrlSchemeHandler>

class CandySchemeHandler : public QWebEngineUrlSchemeHandler
{
public:
    void requestStarted(QWebEngineUrlRequestJob *job) override;
};
