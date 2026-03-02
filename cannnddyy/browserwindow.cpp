#include "browserwindow.h"
#include "candyschemehandler.h"

#include <QToolBar>
#include <QWebEngineProfile>

BrowserWindow::BrowserWindow()
{
    setupUI();

    QWebEngineProfile::defaultProfile()->installUrlSchemeHandler(
        "candy",
        new CandySchemeHandler()
    );

    addNewTab(QUrl("candy://about"));
}

void BrowserWindow::setupUI()
{
    tabs = new QTabWidget(this);
    setCentralWidget(tabs);

    QToolBar *toolbar = addToolBar("Navigation");

    QAction *backAction = toolbar->addAction("←");
    QAction *forwardAction = toolbar->addAction("→");
    QAction *reloadAction = toolbar->addAction("⟳");

    addressBar = new QLineEdit();
    toolbar->addWidget(addressBar);

    connect(addressBar, &QLineEdit::returnPressed, this, [=](){
        QUrl url = QUrl::fromUserInput(addressBar->text());
        QWebEngineView *view = qobject_cast<QWebEngineView*>(tabs->currentWidget());
        if(view)
            view->load(url);
    });

    connect(backAction, &QAction::triggered, this, [=](){
        QWebEngineView *view = qobject_cast<QWebEngineView*>(tabs->currentWidget());
        if(view) view->back();
    });

    connect(forwardAction, &QAction::triggered, this, [=](){
        QWebEngineView *view = qobject_cast<QWebEngineView*>(tabs->currentWidget());
        if(view) view->forward();
    });

    connect(reloadAction, &QAction::triggered, this, [=](){
        QWebEngineView *view = qobject_cast<QWebEngineView*>(tabs->currentWidget());
        if(view) view->reload();
    });
}

void BrowserWindow::addNewTab(const QUrl &url)
{
    QWebEngineView *view = new QWebEngineView();
    view->load(url);

    tabs->addTab(view, "New Tab");

    connect(view, &QWebEngineView::urlChanged, this, [=](const QUrl &url){
        addressBar->setText(url.toString());
    });
}
