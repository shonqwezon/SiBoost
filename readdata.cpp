#include "readdata.h"

QJsonArray ReadData::read(QString path) {
    QJsonArray accs;
    QFile file(path);
    if(file.open(QIODevice::ReadOnly)) {
        QString q = NULL;                           //acc
        while(!file.atEnd()) {
            QJsonObject *acc = new QJsonObject();
            q = file.readLine();
            if (!q.isEmpty() && q[0] != ' ' && q.contains(' ')) {
                QStringList info = q.split(' ');
                acc->insert("login", info.first());
                acc->insert("pass", info.back().trimmed());
                accs.append(*acc);
            }
            delete acc;
        }
        file.close();
        return accs;
    }
    else {
        return {};
    }
}

QString ReadData::getUniqueToken() {
    QString token = QSysInfo::machineUniqueId();
    for(int i = 0; i < token.size(); i++) {
        if(token[i] == '1') token[i] = '-';
        if(token[i] == '6') token[i] = ':';
    }
    return token;
}

QString ReadData::getUniqueId() {
    QString token = QSysInfo::machineUniqueId();
    QString key = QString(QCryptographicHash::hash((token.toUtf8()),QCryptographicHash::Md5).toHex());
    for(int i = 0; i < key.size(); i++) {
        if(key[i] == 'f') key[i] = '/';
        if(key[i] == '2') key[i] = '=';
    }
    return key;
}
