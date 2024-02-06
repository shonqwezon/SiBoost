#ifndef PARAMS_H
#define PARAMS_H

#include <QMap>
#include <QWidget>
#include <QDebug>
#include <QSettings>
#include <QList>

class Params
{
public:
    Params(QMap<int, QString>);
    ~Params();
    QMap<int, QString>* getList();
    bool checkLogin(QString);
    void writeSettings(QString*, QString*, int*, int*, QString*);
    QSettings* getSettings();
    QStringList* info();

private:
    QMap<int, QString>* list;
    QSettings* settings;
};

#endif // PARAMS_H
