#ifndef SETTINGSBRIDGE_H
#define SETTINGSBRIDGE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QSettings>

class SettingsBridge : public QObject
{
    Q_OBJECT
public:
    explicit SettingsBridge(QObject *parent = nullptr);

    Q_INVOKABLE QString getSearchEngine();
    Q_INVOKABLE void setSearchEngine(const QString &engine);

    Q_INVOKABLE QStringList getBlockedSites();
    Q_INVOKABLE void addBlockedSite(const QString &site);
    Q_INVOKABLE void removeBlockedSite(const QString &site);

    Q_INVOKABLE int getTimeLimit();
    Q_INVOKABLE void setTimeLimit(int minutes);

    Q_INVOKABLE QString getParentPin();
    Q_INVOKABLE void setParentPin(const QString &pin);

    Q_INVOKABLE bool getInitialized();
    Q_INVOKABLE void setInitialized(bool val);

private:
    QSettings settings;
};

#endif
