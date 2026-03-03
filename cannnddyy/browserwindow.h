#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QWebChannel>
#include <QTabWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QTimer>
#include "settingsbridge.h"

struct Tab {
    QWebEngineView *view;
    QString url;
    int minutesLeft;
};

class BrowserWindow : public QMainWindow
{
    Q_OBJECT
public:
    BrowserWindow(QWidget *parent=nullptr);

private:
    QTabWidget *tabWidget;
    QLineEdit *urlEdit;
    QComboBox *searchEngineBox;
    SettingsBridge *bridge;
    QList<Tab> tabs;
    QTimer *timer;

    void createToolbar();
    void addNewTab(const QString &url="https://www.bing.com");
    void checkTimeLimits(Tab &tab);
    bool isBlocked(const QString &url);

private slots:
    void navigate();
    void switchTab(int index);
    void closeTab(int index);
    void updateUrlBar(int index);
    void updateSearchEngineLogo();
};

#endif
