#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebChannel>
#include <QTimer>
#include "settingsbridge.h"

class BrowserWindow : public QMainWindow
{
    Q_OBJECT
public:
    BrowserWindow(QWidget *parent=nullptr);

private:
    QWebEngineView *view;
    SettingsBridge *bridge;
    QStringList history;
    QTimer *timer;
    int minutesLeft;

    void loadPage(const QString &url);
    bool isBlocked(const QString &url);
    void checkTimeLimit();

private slots:
    void onLoadStarted();
    void onLoadFinished(bool ok);
};

#endif
