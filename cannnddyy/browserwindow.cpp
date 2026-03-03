#include "browserwindow.h"
#include "candyschemehandler.h"
#include <QShortcut>
#include <QWebEngineProfile>

BrowserWindow::BrowserWindow()
{
    setupUI();

    sessionStart = QTime::currentTime();

    QSettings settings("Houselearning", "Cannnddyy");

    if(!settings.contains("initialized"))
        addNewTab(QUrl("candy://setup"));
    else
        addNewTab();

    QShortcut *historyShortcut = new QShortcut(QKeySequence("Ctrl+H"), this);
    connect(historyShortcut, &QShortcut::activated, this, [=](){
        QWebEngineView *view =
        qobject_cast<QWebEngineView*>(tabs->currentWidget());
        if(view)
            view->load(QUrl("candy://history"));
    });
}

void BrowserWindow::setupUI()
{
    resize(1200, 800);
    setWindowTitle("Cannnddyy");

    setStyleSheet(
        "QMainWindow {"
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #3b82f6, stop:1 #ec4899); }"
    );

    tabs = new QTabWidget();
    tabs->setTabsClosable(true);
    setCentralWidget(tabs);

    connect(tabs, &QTabWidget::tabCloseRequested,
            [=](int index){
        if(tabs->count() > 1)
            tabs->removeTab(index);
    });

    QToolBar *toolbar = addToolBar("Navigation");

    addressBar = new QLineEdit();
    toolbar->addWidget(addressBar);

    engineSelector = new QComboBox();
    engineSelector->addItems({"Bing","DuckDuckGo","Google"});
    toolbar->addWidget(engineSelector);

    currentSearchEngine = "Bing";

    connect(engineSelector,
            &QComboBox::currentTextChanged,
            [=](const QString &text){
        currentSearchEngine = text;
    });

    connect(addressBar, &QLineEdit::returnPressed, [=](){

        QWebEngineView *view =
        qobject_cast<QWebEngineView*>(tabs->currentWidget());

        QString input = addressBar->text();
        QUrl url;

        if(input.startsWith("candy://"))
        {
            url = QUrl(input);
        }
        else if(!input.contains("."))
        {
            if(currentSearchEngine == "Bing")
                url = QUrl("https://www.bing.com/search?q=" + input);
            else if(currentSearchEngine == "DuckDuckGo")
                url = QUrl("https://duckduckgo.com/?q=" + input);
            else
                url = QUrl("https://www.google.com/search?q=" + input);
        }
        else
        {
            url = QUrl::fromUserInput(input);
        }

        for(const QString &blocked : blockedSites)
        {
            if(url.toString().contains(blocked))
            {
                view->load(QUrl("candy://blocked"));
                return;
            }
        }

        if(sessionStart.secsTo(QTime::currentTime()) >
           allowedMinutes * 60)
        {
            view->load(QUrl("candy://timelimit"));
            return;
        }

        view->load(url);
    });
}

void BrowserWindow::addNewTab(QUrl url)
{
    QWebEngineView *view = new QWebEngineView();

    QWebEngineProfile::defaultProfile()
        ->installUrlSchemeHandler("candy",
        new CandySchemeHandler(this));

    tabs->addTab(view, "New Tab");
    tabs->setCurrentWidget(view);

    connect(view, &QWebEngineView::urlChanged,
            [=](const QUrl &url){
        addressBar->setText(url.toString());
    });

    connect(view, &QWebEngineView::titleChanged,
            [=](const QString &title){
        tabs->setTabText(tabs->currentIndex(), title);
        recordHistory(view->url(), title);
    });

    view->load(url);
}

void BrowserWindow::recordHistory(const QUrl &url,
                                  const QString &title)
{
    if(url.scheme() == "http" ||
       url.scheme() == "https")
        historyList.append({title, url.toString()});
}
