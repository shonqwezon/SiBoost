#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QJsonArray>
#include <QScrollArea>
#include <QLabel>
#include <QMap>
#include <QPushButton>
#include <QLineEdit>
#include "accounts.h"
#include <QMessageBox>
#include "params.h"
#include <QSettings>
#include <QSpinBox>
#include "launchgame.h"
#include <windows.h>
#include <QProcess>
#include <QThread>
#include <QTime>
#include <QCloseEvent>
#include <QFile>
#include <QComboBox>
#include <QTimeEdit>
#include <QTimer>
#include <readdata.h>
#include <QClipboard>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    virtual void closeEvent(QCloseEvent*);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setUser(QJsonArray);
    void repaintScrollAdd();
    void startAcc(int);
    void stopAcc(int);

private slots:
    void updateTimer();

    void Delay(int);

    void createScroll();

    void startScroll();

    void settingsScroll();

    void repaintScrollChange(QWidget*);

    void on_startAll_triggered();

    void on_stopAll_triggered();

    void on_settings_triggered();

    void on_edit_triggered();

    void on_timer_triggered();

signals:
    void stop(int);
    void startTimer(int);
    void stopTimer();

private:
    bool key = true;
    int time;
    QTimer* timer;
    QString path;
    bool first = false;
    bool exit = false;
    int num;
    int count;
    Ui::MainWindow *ui;
    Accounts* accounts;
    Params* params;
    bool trigEdit = true;
    bool trigSettings = true;
    struct Config {
        QString path = "";
        QString param = "";
        int timeStart = 5000;
        int timerMode = 0;
        QString key = "";
    };
    Config config;
    QProcess* process;
    QMap<int, QThread*> listThreads;
    QMap<int, LaunchGame*> listGames;
};
#endif // MAINWINDOW_H
