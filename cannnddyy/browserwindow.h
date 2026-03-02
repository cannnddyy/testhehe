#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QWebEngineView>
#include <QWebEngineUrlSchemeHandler>

class BrowserWindow : public QMainWindow
{
    Q_OBJECT

public:
    BrowserWindow();

private:
    QTabWidget *tabs;
    QLineEdit *urlBar;

    void createNewTab(const QUrl &url = QUrl("https://www.bing.com"));
    void setupUI();
    void setupShortcuts();
    void handleCustomUrl(QWebEngineView *view);
};
