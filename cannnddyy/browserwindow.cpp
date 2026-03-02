#include "browserwindow.h"
#include <QToolBar>
#include <QVBoxLayout>
#include <QShortcut>
#include <QFile>
#include <QWebEngineProfile>
#include <QWebEnginePage>

BrowserWindow::BrowserWindow()
{
    setupUI();
    createNewTab();
    setupShortcuts();

    setStyleSheet(
        "QMainWindow {"
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #3b82f6, stop:1 #ec4899);"
        "}"
    );
}

void BrowserWindow::setupUI()
{
    tabs = new QTabWidget();
    tabs->setTabsClosable(true);
    setCentralWidget(tabs);

    connect(tabs, &QTabWidget::tabCloseRequested, this, [=](int index){
        tabs->removeTab(index);
    });

    QToolBar *toolbar = addToolBar("Navigation");

    urlBar = new QLineEdit();
    toolbar->addWidget(urlBar);

    connect(urlBar, &QLineEdit::returnPressed, this, [=](){
        QUrl url = QUrl::fromUserInput(urlBar->text());
        QWebEngineView *view = qobject_cast<QWebEngineView*>(tabs->currentWidget());
        if(view) view->setUrl(url);
    });
}

void BrowserWindow::createNewTab(const QUrl &url)
{
    QWebEngineView *view = new QWebEngineView();
    view->setUrl(url);

    connect(view, &QWebEngineView::urlChanged, this, [=](const QUrl &url){
        urlBar->setText(url.toString());

        // Basic history tracking
        QFile file("history.txt");
        if(file.open(QIODevice::Append)) {
            file.write((url.toString() + "\n").toUtf8());
        }
    });

    tabs->addTab(view, "New Tab");
    tabs->setCurrentWidget(view);
}

void BrowserWindow::setupShortcuts()
{
    new QShortcut(QKeySequence("Ctrl+T"), this, [=](){
        createNewTab();
    });

    new QShortcut(QKeySequence("Ctrl+H"), this, [=](){
        QWebEngineView *view = qobject_cast<QWebEngineView*>(tabs->currentWidget());
        if(view) view->setUrl(QUrl("candy://history"));
    });
}
