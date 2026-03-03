#include "browserwindow.h"
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QFile>
#include <QMessageBox>
#include <QUrl>
#include <QTime>

BrowserWindow::BrowserWindow(QWidget *parent)
    : QMainWindow(parent), minutesLeft(0)
{
    view = new QWebEngineView(this);
    setCentralWidget(view);

    bridge = new SettingsBridge(this);

    QWebChannel *channel = new QWebChannel(view->page());
    channel->registerObject(QStringLiteral("settingsBridge"), bridge);
    view->page()->setWebChannel(channel);

    // First-time setup
    if(!bridge->getInitialized()){
        view->load(QUrl::fromLocalFile("pages/setup.html"));
    } else {
        view->load(QUrl("https://www.bing.com"));
        minutesLeft = bridge->getTimeLimit();
    }

    // Timer to count down minutes
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,[=](){
        if(minutesLeft>0) minutesLeft--;
    });
    timer->start(60000); // every minute

    connect(view,&QWebEngineView::urlChanged,[=](const QUrl &url){
        QString u = url.toString();

        // Candy:// custom pages
        if(u.startsWith("candy://")){
            if(u=="candy://about") view->load(QUrl::fromLocalFile("pages/about.html"));
            else if(u=="candy://history") view->load(QUrl::fromLocalFile("pages/history.html"));
            return;
        }

        // Blocked sites
        if(isBlocked(u)){
            view->load(QUrl::fromLocalFile("pages/blocked.html"));
            return;
        }

        // Time limit
        if(minutesLeft<=0){
            view->load(QUrl::fromLocalFile("pages/timelimit.html"));
            return;
        }

        // Add to history
        history.append(u);
    });
}

void BrowserWindow::loadPage(const QString &url){
    view->load(QUrl(url));
}

bool BrowserWindow::isBlocked(const QString &url){
    QStringList blocked = bridge->getBlockedSites();
    for(const QString &b : blocked){
        if(url.contains(b)) return true;
    }
    return false;
}
