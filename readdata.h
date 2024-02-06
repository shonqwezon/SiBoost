#ifndef READDATA_H
#define READDATA_H

#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QSysInfo>
#include <QCryptographicHash>

class ReadData
{
public:
    static QJsonArray read(QString);
    static QString getUniqueToken();
    static QString getUniqueId();
};

#endif // READDATA_H
