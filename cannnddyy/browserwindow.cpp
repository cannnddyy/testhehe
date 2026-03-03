#include "browserwindow.h"
#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QUrl>
#include <QFile>
#include <QWebEnginePage>
#include <QMessageBox>

BrowserWindow::BrowserWindow(QWidget *parent)
    : QMainWindow(parent)
{
    bridge = new SettingsBridge(this);

    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    setCentralWidget(tabWidget);

    createToolbar();

    connect(tabWidget, &QTabWidget::currentChanged, this, &BrowserWindow::switchTab);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &BrowserWindow::closeTab);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
        for(auto &t : tabs){
            if(t.minutesLeft > 0) t.minutesLeft--;
        }
    });
    timer->start(60000); // 1 minute

    if(!bridge->getInitialized()){
        addNewTab(QUrl::fromLocalFile("pages/setup.html").toString());
    } else {
        addNewTab();
    }
}

void BrowserWindow::createToolbar()
{
    QToolBar *toolbar = new QToolBar(this);
    addToolBar(toolbar);

    urlEdit = new QLineEdit(this);
    toolbar->addWidget(urlEdit);

    QPushButton *goBtn = new QPushButton("Go", this);
    toolbar->addWidget(goBtn);
    connect(goBtn, &QPushButton::clicked, this, &BrowserWindow::navigate);

    QPushButton *newTabBtn = new QPushButton("+", this);
    toolbar->addWidget(newTabBtn);
    connect(newTabBtn, &QPushButton::clicked, [=](){ addNewTab(); });

    searchEngineBox = new QComboBox(this);
    searchEngineBox->addItems({"Bing","DuckDuckGo","Google"});
    searchEngineBox->setCurrentText(bridge->getSearchEngine());
    toolbar->addWidget(searchEngineBox);
    connect(searchEngineBox, &QComboBox::currentTextChanged, [=](const QString &){
        bridge->setSearchEngine(searchEngineBox->currentText());
        updateSearchEngineLogo();
    });
}

void BrowserWindow::addNewTab(const QString &url)
{
    QWebEngineView *view = new QWebEngineView(this);
    QWebChannel *channel = new QWebChannel(view->page());
    channel->registerObject("settingsBridge", bridge);
    view->page()->setWebChannel(channel);

    Tab tab;
    tab.view = view;
    tab.url = url;
    tab.minutesLeft = bridge->getTimeLimit();
    tabs.append(tab);

    tabWidget->addTab(view, "New Tab");

    view->load(QUrl(url));

    connect(view, &QWebEngineView::urlChanged, [=](const QUrl &u){
        QString s = u.toString();
        tab.url = s;

        if(isBlocked(s)){
            view->load(QUrl::fromLocalFile("pages/blocked.html"));
        } else if(tab.minutesLeft <= 0){
            view->load(QUrl::fromLocalFile("pages/timelimit.html"));
        }

        updateUrlBar(tabWidget->currentIndex());
    });

    tabWidget->setCurrentWidget(view);
}

void BrowserWindow::navigate()
{
    QString u = urlEdit->text();
    if(!u.startsWith("http") && !u.startsWith("candy://")) u = "https://"+u;

    QWebEngineView *view = tabs[tabWidget->currentIndex()].view;
    view->load(QUrl(u));
}

void BrowserWindow::switchTab(int index)
{
    updateUrlBar(index);
}

void BrowserWindow::closeTab(int index)
{
    QWidget *w = tabWidget->widget(index);
    tabWidget->removeTab(index);
    w->deleteLater();
    tabs.removeAt(index);
}

void BrowserWindow::updateUrlBar(int index)
{
    if(index < 0 || index >= tabs.size()) return;
    urlEdit->setText(tabs[index].url);
}

bool BrowserWindow::isBlocked(const QString &url)
{
    QStringList blocked = bridge->getBlockedSites();
    for(const QString &b : blocked){
        if(url.contains(b)) return true;
    }
    return false;
}

void BrowserWindow::updateSearchEngineLogo()
{
    // You can later swap a QLabel with a logo depending on engine
}
