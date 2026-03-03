#include "browserwindow.h"
#include <QToolBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QWebEngineView>
#include <QWebChannel>
#include <QDir>
#include <QTimer>
#include <QCoreApplication>

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

    // timer for time limits
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
        for (auto &t : tabs) {
            if (t.minutesLeft > 0) t.minutesLeft--;
        }
    });
    timer->start(60000); // every minute

    // first launch
    QString setupPage = QDir(QCoreApplication::applicationDirPath()).filePath("cannnddyy/pages/setup.html");
    if (!bridge->getInitialized()) {
        addNewTab(QUrl::fromLocalFile(setupPage).toString());
    } else {
        addNewTab(); // default search engine
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
    connect(newTabBtn, &QPushButton::clicked, [=]() { addNewTab(); });

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

    // web channel for settings bridge
    QWebChannel *channel = new QWebChannel(view->page());
    channel->registerObject("settingsBridge", bridge);
    view->page()->setWebChannel(channel);

    QString startUrl = url.isEmpty() ? "https://www.bing.com" : url;

    Tab tab;
    tab.view = view;
    tab.url = startUrl;
    tab.minutesLeft = bridge->getTimeLimit();
    tabs.append(tab);

    tabWidget->addTab(view, "New Tab");

    view->load(QUrl(startUrl));

    connect(view, &QWebEngineView::urlChanged, [=](const QUrl &u){
        QString s = u.toString();
        tab.url = s;

        // handle candy:// urls
        if (s.startsWith("candy://")) {
            QString pageFile;
            if (s == "candy://about") pageFile = "about.html";
            else if (s == "candy://history") pageFile = "history.html";
            else if (s == "candy://settings") pageFile = "settings.html";
            else if (s == "candy://blocked") pageFile = "blocked.html";
            else if (s == "candy://timelimit") pageFile = "timelimit.html";
            else if (s == "candy://setup") pageFile = "setup.html";
            else return;

            QString fullPath = QDir(QCoreApplication::applicationDirPath()).filePath("cannnddyy/pages/" + pageFile);
            view->load(QUrl::fromLocalFile(fullPath));
            return;
        }

        // blocked sites
        if (isBlocked(s)) {
            QString fullPath = QDir(QCoreApplication::applicationDirPath()).filePath("cannnddyy/pages/blocked.html");
            view->load(QUrl::fromLocalFile(fullPath));
            return;
        }

        // time limit reached
        if (tab.minutesLeft <= 0) {
            QString fullPath = QDir(QCoreApplication::applicationDirPath()).filePath("cannnddyy/pages/timelimit.html");
            view->load(QUrl::fromLocalFile(fullPath));
            return;
        }

        updateUrlBar(tabWidget->currentIndex());
    });

    tabWidget->setCurrentWidget(view);
}

void BrowserWindow::navigate()
{
    QString u = urlEdit->text();
    if (!u.startsWith("http") && !u.startsWith("candy://")) u = "https://" + u;

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
    if (index < 0 || index >= tabs.size()) return;
    urlEdit->setText(tabs[index].url);
}

bool BrowserWindow::isBlocked(const QString &url)
{
    QStringList blocked = bridge->getBlockedSites();
    for (const QString &b : blocked) {
        if (url.contains(b)) return true;
    }
    return false;
}

void BrowserWindow::updateSearchEngineLogo()
{
    // optional: dynamically update toolbar logo depending on selected search engine
}
