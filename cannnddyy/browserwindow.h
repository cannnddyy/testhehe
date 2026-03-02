#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QWebEngineView>

class BrowserWindow : public QMainWindow
{
    Q_OBJECT

public:
    BrowserWindow();

private:
    QTabWidget *tabs;
    QLineEdit *addressBar;

    void setupUI();
    void addNewTab(const QUrl &url);
};
