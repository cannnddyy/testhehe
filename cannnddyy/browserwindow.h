#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QToolBar>
#include <QComboBox>
#include <QVector>
#include <QPair>
#include <QTime>
#include <QSettings>
#include <QtWebEngineWidgets/QWebEngineView>

class BrowserWindow : public QMainWindow
{
    Q_OBJECT

public:
    BrowserWindow();

    QVector<QPair<QString, QString>> historyList;

private:
    QTabWidget *tabs;
    QLineEdit *addressBar;
    QComboBox *engineSelector;

    QString currentSearchEngine;
    QStringList blockedSites;

    QTime sessionStart;
    int allowedMinutes = 60;

    void setupUI();
    void addNewTab(QUrl url = QUrl("candy://about"));
    void recordHistory(const QUrl &url, const QString &title);
};

#endif
