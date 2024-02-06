#ifndef LAUNCHGAME_H
#define LAUNCHGAME_H

#include <QThread>
#include <QDebug>
#include <QProcess>


class LaunchGame : public QThread
{
    Q_OBJECT
public:
    explicit LaunchGame(QString, QString, QString);
    ~LaunchGame();
    void start();
    void close(int);

signals: void finished(int);

private:
    QString command;
    int id;
    QProcess* proc;
};

#endif // LAUNCHGAME_H
