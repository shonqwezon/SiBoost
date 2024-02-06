#include "params.h"

Params::Params(QMap<int, QString> list) {
    this->list = new QMap<int, QString>(list);
    settings = new QSettings();
}

Params::~Params() { }

QMap<int, QString>* Params::getList() { return list; }

bool Params::checkLogin(QString login) {
    foreach(int pos, list->keys()) {
        if(list->value(pos).split(' ').first() == login) {
            return false;
        }
    }
    return true;
}

QSettings* Params::getSettings() {
    return settings;
}

void Params::writeSettings(QString* path, QString* param, int* accStart, int* timerMode, QString* key) {
    settings->beginGroup("settings");
    settings->setValue("path", *path);
    settings->setValue("param", *param);
    settings->setValue("accStart", *accStart);
    settings->setValue("timerMode", *timerMode);
    settings->setValue("key", *key);
    settings->endGroup();
    settings->sync();
}

QStringList* Params::info() {
    QStringList* config = new QStringList();
    config->append(settings->value("settings/path", NULL).toString());
    config->append(settings->value("settings/param", NULL).toString());
    config->append(settings->value("settings/accStart", NULL).toString());
    config->append(settings->value("settings/timerMode", NULL).toString());
    config->append(settings->value("settings/key", NULL).toString());
    return config;
}

