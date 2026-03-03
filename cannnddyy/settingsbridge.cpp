#include "settingsbridge.h"

SettingsBridge::SettingsBridge(QObject *parent)
    : QObject(parent), settings("Houselearning","Cannnddyy")
{
    if(!settings.contains("initialized"))
        settings.setValue("initialized", false);
}

QString SettingsBridge::getSearchEngine() { return settings.value("searchEngine","Bing").toString(); }
void SettingsBridge::setSearchEngine(const QString &engine) { settings.setValue("searchEngine",engine); }

QStringList SettingsBridge::getBlockedSites() { return settings.value("blockedSites").toStringList(); }
void SettingsBridge::addBlockedSite(const QString &site){
    QStringList list = settings.value("blockedSites").toStringList();
    if(!list.contains(site)) list.append(site);
    settings.setValue("blockedSites",list);
}
void SettingsBridge::removeBlockedSite(const QString &site){
    QStringList list = settings.value("blockedSites").toStringList();
    list.removeAll(site);
    settings.setValue("blockedSites",list);
}

int SettingsBridge::getTimeLimit() { return settings.value("timeLimit",60).toInt(); }
void SettingsBridge::setTimeLimit(int minutes) { settings.setValue("timeLimit",minutes); }

QString SettingsBridge::getParentPin() { return settings.value("parentPin","").toString(); }
void SettingsBridge::setParentPin(const QString &pin){ settings.setValue("parentPin",pin); }

bool SettingsBridge::getInitialized(){ return settings.value("initialized",false).toBool(); }
void SettingsBridge::setInitialized(bool val){ settings.setValue("initialized",val); }
