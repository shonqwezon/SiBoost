#include "launchgame.h"

LaunchGame::LaunchGame(QString path, QString param, QString info) {
    command = path + "\\steam.exe \"steam://run/730//" + param + "/\" -login " + info;
}

LaunchGame::~LaunchGame() {
    proc->close();
    proc->waitForFinished();
    delete proc;
    qDebug() << "exit game";
}

void LaunchGame::start() {
    proc = new QProcess();
    proc->startCommand(command);
    proc->waitForStarted();
    id = proc->processId();
    qDebug() << "started" << id;
}

void LaunchGame::close(int pos) {
    qDebug() << "all close game";
    QProcess* procEnd = new QProcess();
    procEnd->startCommand("taskkill /PID " + QString::number(id) + " /f /t");
    procEnd->waitForFinished();
    procEnd->close();
    delete procEnd;
    emit finished(pos);
}


