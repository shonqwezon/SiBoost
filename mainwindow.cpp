#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    path = QCoreApplication::applicationDirPath()+"/data.txt";
}

MainWindow::~MainWindow() {
    qDebug() << "the end 2";
    delete accounts;
    delete params;
    delete ui;
}

void MainWindow::repaintScrollChange(QWidget* widget) {
    ui->scrollArea->setWidget(widget);
}

void MainWindow::setUser(QJsonArray object) {
    QJsonArray mObject = object;
    QMap<int, QString> list;
    for(int i = 0; i < mObject.size(); i++) {
        QJsonObject* object = new QJsonObject(mObject[i].toObject());
        list.insert(i+1, object[0].value("login").toString() + " " + object[0].value("pass").toString());
    }

    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    timer->setInterval(60000);
    QObject::connect(this, &MainWindow::startTimer, [this](int s){
        ui->statusbar->showMessage("До выключения: " + QString::number(s) + " минут/ы!");
        time = s;
        timer->start();
    });
    QObject::connect(this, &MainWindow::stopTimer, [this](){
        timer->stop();
    });

    params = new Params(list);
    accounts = new Accounts(params);
    count = 0;
    num = 0;
    ui->stopAll->setEnabled(false);

    QObject::connect(accounts, &Accounts::sendRepaintChange, this, &MainWindow::repaintScrollChange);
    QObject::connect(accounts, &Accounts::onAcc, this, &MainWindow::startAcc);
    QObject::connect(accounts, &Accounts::offAcc, this, &MainWindow::stopAcc);
    QObject::connect(this, &MainWindow::stop, [this] (int id) {
        listGames.value(id)->close(id);
    });

    if(params->getSettings()->childGroups().isEmpty()) {
        first = true;
        MainWindow::on_settings_triggered();
    }
    else {
        //тут получаем структуру
        config = { params->info()->takeAt(0), params->info()->takeAt(1), params->info()->takeAt(2).toInt(), params->info()->takeAt(3).toInt(),  params->info()->takeAt(4)};
        if(!QFile::exists(config.path + "//steam.exe")) {
            QMessageBox::critical(0, "Ошибка!", "\"steam.exe\" в папке не обнаружен!\nЗаново настройте программу!");
            first = true;
            MainWindow::on_settings_triggered();
        }
        else {
            if(config.key != ReadData::getUniqueId()) {
                key = false;
                while(params->getList()->size() > 2) {
                    params->getList()->remove(params->getList()->lastKey());
                }
            }
            MainWindow::startScroll();
        }
    }
}

void MainWindow::startAcc(int pos) {
    count++;
    accounts->buttonsState(pos, num);
    QThread* thread = new QThread;
    LaunchGame* game = new LaunchGame(config.path, config.param, params->getList()->value(pos));
    game->moveToThread(thread);
    listThreads.insert(pos, thread);
    listGames.insert(pos, game);
    QObject::connect(thread, &QThread::started, [this, pos] {
        qDebug() << "started" << pos;
        listGames.value(pos)->start();
    });
    QObject::connect(game, &LaunchGame::finished, [this, pos] {
        listGames.value(pos)->deleteLater();
    });
    QObject::connect(game, &LaunchGame::finished, [this, pos] {
        listThreads.value(pos)->quit();
    });
    QObject::connect(thread, &QThread::finished, [this, pos] {
        listThreads.value(pos)->deleteLater();
    });
    QObject::connect(thread, &QThread::destroyed, [pos]() {
        qDebug() << "close" << pos;
    });

    thread->start();
    if(count > 0) {
        if(count == params->getList()->size()) {
            ui->startAll->setEnabled(false);
        }
        ui->stopAll->setEnabled(true);
        ui->edit->setEnabled(false);
        ui->settings->setEnabled(false);
    }
    Delay(config.timeStart);
}


void MainWindow::stopAcc(int pos) {
    count--;
    accounts->buttonsState(pos, num);
    emit stop(pos);
    if(count < params->getList()->size()) {
        ui->startAll->setEnabled(true);
        if(count == 0) {
            ui->edit->setEnabled(true);
            ui->settings->setEnabled(true);
            ui->stopAll->setEnabled(false);
        }
    }
}

void MainWindow::on_startAll_triggered()
{
    exit = false;
    ui->startAll->setEnabled(false);
    foreach(int pos, params->getList()->keys()) {
        if(exit) {
            exit = false;
            break;
        }
        accounts->checkState(pos, 1, num);
    }
}

void MainWindow::on_stopAll_triggered()
{
    ui->stopAll->setEnabled(false);
    exit = true;
    foreach(int pos, params->getList()->keys()) {
        accounts->checkState(pos, 0, num);
        Delay(500);
        qDebug() << pos << "yes";
    }
}

void MainWindow::on_settings_triggered()
{
    if(trigSettings) {
        MainWindow::settingsScroll();
        ui->startAll->setEnabled(false);
    }
    else {
        if(first) {
            QMessageBox::information(0, QCoreApplication::applicationName(), "Настрой программу прежде чем начать буст!");
            trigSettings = true;
        }
        else {
            MainWindow::startScroll();
        }
    }
    ui->edit->setEnabled(!trigSettings);
    trigSettings = !trigSettings;
}

void MainWindow::settingsScroll() {
    QString* keyStr = new QString(config.key);
    QWidget* widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();

    QLabel* labelPath = new QLabel("Путь к директории steam.exe");
    QLineEdit* editPath = new QLineEdit();
    editPath->setText(config.path);
    editPath->setMinimumWidth(400);
    QLabel* labelParam = new QLabel("Параметры запуска CS:GO");
    QLineEdit* editParam = new QLineEdit();
    editParam->setText(config.param);
    editParam->setMinimumWidth(400);
    QLabel* labelTimings = new QLabel("Время");
    QLabel* labelOn = new QLabel("Тайминги запуска CS:GO:");
    QSpinBox* spinOn = new QSpinBox();
    spinOn->setRange(100, 10000);
    spinOn->setSingleStep(100);
    spinOn->setValue(config.timeStart);

    QLabel* labelTimer = new QLabel("Параметры таймера:");
    QComboBox* combobox = new QComboBox();
    combobox->addItem("Закрыть программу");
    combobox->addItem("Закрыть программу + спящий режим");
    combobox->addItem("Закрыть программу + выключить ПК");
    combobox->setCurrentIndex(config.timerMode);

    layout->addWidget(labelPath);
    layout->addWidget(editPath);
    layout->addWidget(labelParam);
    layout->addWidget(editParam);
    layout->addWidget(labelTimings);
    QWidget* widgetH1 = new QWidget();
    QHBoxLayout* layoutH1 = new QHBoxLayout(widgetH1);
    layoutH1->addWidget(labelOn);
    layoutH1->addWidget(spinOn);
    layout -> addWidget(widgetH1);

    QWidget* widgetH2 = new QWidget();
    QHBoxLayout* layoutH2 = new QHBoxLayout(widgetH2);
    layoutH2->addWidget(labelTimer);
    layoutH2->addWidget(combobox);
    widgetH2 -> setLayout(layoutH2);
    layout -> addWidget(widgetH2);

    QWidget* widgetH3 = new QWidget();
    QHBoxLayout* layoutH3 = new QHBoxLayout(widgetH3);

    QPushButton* buttonSave = new QPushButton();
    buttonSave->setText("Сохранить");
    layoutH3->addWidget(buttonSave);

    QPushButton* buttonExit = new QPushButton();
    buttonExit->setText("Выйти");
    layoutH3->addWidget(buttonExit);

    QPushButton* buttonKey = new QPushButton();
    buttonKey->setText("Ввести ключ");
    layoutH3->addWidget(buttonKey);

    layout -> addWidget(widgetH3);
    widget->setLayout(layout);

    QObject::connect(buttonKey, &QPushButton::clicked, [this, keyStr](){
        QDialog *dialog = new QDialog();
        QGridLayout* layoutG = new QGridLayout(dialog);
        layoutG->setVerticalSpacing(8);
        layoutG->setHorizontalSpacing(8);
        QLabel* label = new QLabel("Ключ:");
        QLineEdit* lineedit = new QLineEdit();
        QPushButton *buttonAccept = new QPushButton();
        buttonAccept->setText("Ввести");
        QPushButton *buttonCancel = new QPushButton();
        buttonCancel->setText("Отмена");
        QPushButton *buttonToken = new QPushButton();
        buttonToken->setText("Токен");
        layoutG->addWidget(label, 0, 0);
        layoutG->addWidget(lineedit, 0, 1, 1, 5);
        layoutG->addWidget(buttonAccept, 1, 2);
        layoutG->addWidget(buttonCancel, 1, 3);
        layoutG->addWidget(buttonToken, 1, 4);
        QObject::connect(buttonToken, &QPushButton::clicked, [] {
            QMessageBox::information(0, QCoreApplication::applicationName(), "Токен скопирован!");
            QString token = ReadData::getUniqueToken();
            QApplication::clipboard()->setText(token);
        });
        QObject::connect(buttonCancel, &QPushButton::clicked, dialog, &QDialog::reject);
        QObject::connect(buttonAccept, &QPushButton::clicked, [lineedit, dialog, keyStr, this] () {
            if(lineedit->text() == ReadData::getUniqueId()) {
                QMessageBox::information(0, QCoreApplication::applicationName(), "Ключ валиден, не забудьте сохранить настройки!");
                *keyStr = lineedit->text();
                key = true;
                dialog->accept();
            }
            else {
                QMessageBox::information(0, QCoreApplication::applicationName(), "Инвалидный ключ!");
            }
        });
        if(dialog->exec() == QDialog::Accepted) {
            qDebug() << "active";
        }

    });

    QObject::connect(buttonExit, &QPushButton::clicked, [this](){
        if(QFile::exists(path)){
            QFile::remove(path);
        }
        if(QProcess::startDetached(QApplication::applicationFilePath(), QStringList(), QApplication::applicationDirPath())) {
            QApplication::quit();
        }
        else {
            QMessageBox::critical(0, "Ошибка!", "Невозможно перезапустить приложение!");
        }
    });

    QObject::connect(buttonSave, &QPushButton::clicked, [this, editPath, editParam, spinOn, combobox, keyStr] () {
        QString* path = new QString(editPath->text());
        QString* param = new QString(editParam->text());
        int* intOn = new int(spinOn->value());
        int* i = new int(combobox->currentIndex());
        if(!path->isEmpty()) {
            if(QFile::exists(*path + "\\steam.exe")) {
                params->writeSettings(path, param, intOn, i, keyStr);
                config = {*path, *param, *intOn, *i, *keyStr};
                delete keyStr;
                QMessageBox::information(0, QCoreApplication::applicationName(), "Вы успешно изменили настройки!");
                if(first) {
                    if(config.key != ReadData::getUniqueId()) {
                        key = false;
                        while(params->getList()->size() > 2) {
                            params->getList()->remove(params->getList()->lastKey());
                        }
                    }
                    first = false;
                }
                trigSettings = false;
                MainWindow::on_settings_triggered();
            }
            else {
                QMessageBox::critical(0, "Ошибка!", "В данной директории \"steam.exe\" не обнаружен");
            }
        }
        else {
            QMessageBox::critical(0, "Ошибка!", "Заполните поле \"Путь к директории steam.exe\"");
        }
        delete i;
        delete intOn;
        delete path;
        delete param;
    });
    ui->scrollArea->setWidget(widget);
}

void MainWindow::on_edit_triggered() {
    if(trigEdit) {
        MainWindow::createScroll();
        ui->startAll->setEnabled(false);
    }
    else {
        MainWindow::startScroll();
    }
    ui->settings->setEnabled(!trigEdit);
    trigEdit = !trigEdit;
}

void MainWindow::on_timer_triggered() {
    QDialog *dialog = new QDialog();

    QGridLayout* layout = new QGridLayout(dialog);
    layout->setVerticalSpacing(8);
    layout->setHorizontalSpacing(8);
    if(timer->isActive()) {
        QLabel* label = new QLabel("Сбросить таймер?");
        QPushButton *buttonAccept = new QPushButton();
        buttonAccept->setText("Да");
        QPushButton *buttonCancel = new QPushButton();
        buttonCancel->setText("Нет");
        layout->addWidget(label, 0, 0);
        layout->addWidget(buttonAccept, 0, 1);
        layout->addWidget(buttonCancel, 0, 2);
        QObject::connect(buttonCancel, &QPushButton::clicked, dialog, &QDialog::reject);
        QObject::connect(buttonAccept, &QPushButton::clicked, [this, dialog] () {
            emit stopTimer();
            ui->statusbar->showMessage("");
            dialog->accept();
        });
    }
    else {
        QLabel *labelTimer = new QLabel();
        labelTimer->setText("Длительность таймера\n(часы/минуты):");
        QTimeEdit* timeEdit = new QTimeEdit();
        timeEdit->setMinimumTime(QTime(0, 1, 0));
        timeEdit->setTime(QTime(0, 30, 0));

        QPushButton *buttonAccept = new QPushButton();
        buttonAccept->setText("Принять");
        QPushButton *buttonCancel = new QPushButton();
        buttonCancel->setText("Отменить");
        layout->addWidget(labelTimer, 0, 0);
        layout->addWidget(timeEdit, 0, 1, 1, 3);
        layout->addWidget(buttonAccept, 1, 1);
        layout->addWidget(buttonCancel, 1, 2);
        QObject::connect(buttonCancel, &QPushButton::clicked, dialog, &QDialog::reject);
        QObject::connect(buttonAccept, &QPushButton::clicked, [this, dialog, timeEdit] () {
            emit startTimer(timeEdit->time().hour() * 60 + timeEdit->time().minute());
            dialog->accept();
        });
    }
    dialog->exec();
}

void MainWindow::updateTimer() {
    time--;
    if(time == 0) {
        emit stopTimer();
        switch(config.timerMode) {
            case 0:
                QApplication::quit();
                break;
            case 1:
                system("rundll32 powrprof.dll,SetSuspendState 0,1,0");
                QApplication::quit();
                break;
            case 2:
                system("shutdown -s -t 10");
                QApplication::quit();
                break;
            default:
                break;
        }
    }
    ui->statusbar->showMessage("До выключения: " + QString::number(time) + " минут/ы!");

}

void MainWindow::startScroll() {
    num++;
    if(!params->getList()->isEmpty()) {
        ui->startAll->setEnabled(true);
        QVBoxLayout* layout = new QVBoxLayout();
        QWidget* widget = new QWidget();
        foreach(int pos, params->getList()->keys()) {
            QStringList info = params->getList()->value(pos).split(' ');
            accounts->mainAccs(layout, pos, info.first(), info.back(), num);
        }
        widget->setLayout(accounts->getSLayout());
        ui->scrollArea->setWidget(widget);
    }
    else {
        ui->scrollArea->setWidget(new QLabel("Добавьте аккаунт в разделе \"Редактирование\""));
        ui->startAll->setEnabled(false);
    }
}

void MainWindow::createScroll() {
    QVBoxLayout* layout = new QVBoxLayout();

    QPushButton *addButton = new QPushButton();
    addButton->setMinimumWidth(400);
    addButton->setText("Добавить новый аккаунт");
    QObject::connect(addButton, &QPushButton::clicked, [this]() {
        if(key) {
            accounts->addNewAcc();
        }
        else {
            if(params->getList()->size() < 2) accounts->addNewAcc();
            else {
                QMessageBox::information(0, QCoreApplication::applicationName(), "Вы не можете использовать больше двух аккаунтов!\nЧтобы обойти данное ограничение купите ключ!\nПодробнее во вкалдке \"Настройка\"");
            }
        }

    });
    QObject::connect(accounts, &Accounts::sendCreateScroll, this, &MainWindow::createScroll);

    accounts->addMainWidgets(addButton);
    layout->addWidget(addButton);

    QWidget* widget = new QWidget();
    if(!params->getList()->isEmpty()) {
        foreach(int pos, params->getList()->keys()) {
            QStringList info = params->getList()->value(pos).split(' ');
            accounts->changeAccs(layout, pos, info.first(), info.back());
        }
        widget->setLayout(accounts->getCLayout());
    }
    else { widget->setLayout(layout); }

    ui->scrollArea->setWidget(widget);
}

void MainWindow::Delay(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs(millisecondsToWait);
    while(QTime::currentTime() < dieTime && !exit)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
    }
}

void MainWindow::closeEvent(QCloseEvent*) {
    qDebug() << "the end 1";
    delete timer;
    if(count != 0) {
        on_stopAll_triggered();
    }
}
