#include "accounts.h"

Accounts::Accounts(Params* params) {
    this->params = params;
}

Accounts::~Accounts() { }

void Accounts::addMainWidgets(QPushButton* button) { list.insert(0, button); }

QVBoxLayout* Accounts::getCLayout() {
    return cLayout;
}

QVBoxLayout* Accounts::getSLayout() {
    return sLayout;
}

void Accounts::changeAcc(int pos) {
    QStringList* info = new QStringList(params->getList()->value(pos).split(' '));
    QString* oldLog = new QString(info->first());
    QString* oldPas = new QString(info->back());
    QDialog *dialog = new QDialog();

    QGridLayout* layout = new QGridLayout(dialog);
    layout->setVerticalSpacing(8);
    layout->setHorizontalSpacing(8);
    QLabel *labelLog = new QLabel();
    labelLog->setText("Логин:");
    QLabel *labelPas = new QLabel();
    labelPas->setText("Пароль:");
    QLineEdit *editLog = new QLineEdit();
    editLog->setText(info->first());
    QLineEdit *editPas = new QLineEdit();
    editPas->setText(info->back());
    QPushButton *buttonAccept = new QPushButton();
    buttonAccept->setText("Изменить");
    QPushButton *buttonCancel = new QPushButton();
    buttonCancel->setText("Отменить");
    layout->addWidget(labelLog, 0, 0);
    layout->addWidget(editLog, 0, 1, 1, 3);
    layout->addWidget(labelPas, 1, 0);
    layout->addWidget(editPas, 1, 1, 1, 3);
    layout->addWidget(buttonAccept, 2, 1);
    layout->addWidget(buttonCancel, 2, 2);
    QObject::connect(buttonCancel, &QPushButton::clicked, dialog, &QDialog::reject);
    QObject::connect(buttonAccept, &QPushButton::clicked, [dialog, editLog, editPas] () {
        if(!editLog->text().isEmpty() && !editPas->text().isEmpty()) dialog->accept();
    });

    if(dialog->exec() == QDialog::Accepted) {
        if(*oldLog != editLog->text() || *oldPas != editPas->text()) {
            if(params->checkLogin(editLog->text())) {
                QString info = editLog->text() + " " + editPas->text();
                params->getList()->insert(pos, info);
                emit sendCreateScroll();
            }
            else {
                QMessageBox::critical(0, "Ошибка!", "Аккаунт с данным логином уже добавлен");
            }
        }
    }
}

void Accounts::addNewAcc() {
    qDebug() << "click!";
    QDialog *dialog = new QDialog();

    QGridLayout* layout = new QGridLayout(dialog);
    layout->setVerticalSpacing(8);
    layout->setHorizontalSpacing(8);
    QLabel *labelLog = new QLabel();
    labelLog->setText("Логин:");
    QLabel *labelPas = new QLabel();
    labelPas->setText("Пароль:");
    QLineEdit *editLog = new QLineEdit();
    QLineEdit *editPas = new QLineEdit();
    QPushButton *buttonAccept = new QPushButton();
    buttonAccept->setText("Принять");
    QPushButton *buttonCancel = new QPushButton();
    buttonCancel->setText("Отменить");
    layout->addWidget(labelLog, 0, 0);
    layout->addWidget(editLog, 0, 1, 1, 3);
    layout->addWidget(labelPas, 1, 0);
    layout->addWidget(editPas, 1, 1, 1, 3);
    layout->addWidget(buttonAccept, 2, 1);
    layout->addWidget(buttonCancel, 2, 2);
    QObject::connect(buttonCancel, &QPushButton::clicked, dialog, &QDialog::reject);
    QObject::connect(buttonAccept, &QPushButton::clicked, [dialog, editLog, editPas] () {
        if(!editLog->text().isEmpty() && !editPas->text().isEmpty()) dialog->accept();
    });

    if(dialog->exec() == QDialog::Accepted) {
        if(params->checkLogin(editLog->text())) {
            QList<int>* pos = new QList(params->getList()->keys());
            QString info = editLog->text() + " " + editPas->text();

            if(pos->isEmpty()) params->getList()->insert(1, info);
            else{ params->getList()->insert(pos->back()+1, info);}

            emit sendCreateScroll();
        }
        else {
            QMessageBox::critical(0, "Ошибка!", "Аккаунт с данным логином уже добавлен");
        }
    }
}

void Accounts::changeAccs(QVBoxLayout* layout, int pos, QString logStr, QString passStr) {
    cLayout = layout;

    QWidget* widget = new QWidget();

    QHBoxLayout* boxLayout = new QHBoxLayout(widget);
    QLineEdit *login = new QLineEdit();
    QLineEdit *password = new QLineEdit();
    login->setText(logStr);
    password->setText(passStr);
    login->setReadOnly(true);
    password->setReadOnly(true);

    boxLayout->addWidget(login);
    boxLayout->addWidget(password);

    QPushButton *editButton = new QPushButton();
    editButton->setText("Изменить");

    QPushButton *deleteButton = new QPushButton();
    deleteButton->setText("Удалить");

    boxLayout->addWidget(editButton);
    boxLayout->addWidget(deleteButton);

    cLayout->addWidget(widget);

    QObject::connect(deleteButton, &QPushButton::clicked, [this, pos] () {
        Accounts::deleteAcc(pos);
    });
    QObject::connect(editButton, &QPushButton::clicked, [this, pos] () {
        Accounts::changeAcc(pos);
    });

    list.insert(pos, widget);

}

void Accounts::deleteAcc(int pos) {
    list.remove(pos);
    params->getList()->remove(pos);
    QWidget* wWidget = new QWidget();
    QVBoxLayout* wLayout = new QVBoxLayout(wWidget);
    qDebug() << list;
    foreach(QWidget* widget, list) {
        wLayout->addWidget(widget);
    }
    emit sendRepaintChange(wWidget);
}

void Accounts::mainAccs(QVBoxLayout* layout, int pos, QString logStr, QString passStr, int m_num) {
    sLayout = layout;
    QWidget* widget = new QWidget();

    QHBoxLayout* boxLayout = new QHBoxLayout(widget);
    QLineEdit *login = new QLineEdit();
    QLineEdit *password = new QLineEdit();
    login->setText(logStr);
    password->setText(passStr);
    login->setReadOnly(true);
    login->setFocusPolicy(Qt::NoFocus);
    password->setReadOnly(true);
    password->setFocusPolicy(Qt::NoFocus);
    boxLayout->addWidget(login);
    boxLayout->addWidget(password);

    QPushButton *startButton = new QPushButton();
    startButton->setText("Запустить");
    startButton->setEnabled(true);

    QPushButton *stopButton = new QPushButton();
    stopButton->setText("Остановить");
    stopButton->setEnabled(false);

    boxLayout->addWidget(startButton);
    boxLayout->addWidget(stopButton);

    QObject::connect(startButton, &QPushButton::clicked, [this, pos] () {
        emit onAcc(pos);
    });
    QObject::connect(stopButton, &QPushButton::clicked, [this, pos] () {
        emit offAcc(pos);
    });
    QObject::connect(this, &Accounts::checkStateButton, [this, startButton, stopButton, pos, m_num] (int id, int mode, int num) {
        if(m_num == num) {
            if(id == pos) {
                if(mode == 0) {
                    if(stopButton->isEnabled()) {
                        qDebug() << "off" << id;
                        emit offAcc(id);
                    }
                }
                else {
                    if(startButton->isEnabled()) {
                        qDebug() << "on" << id;
                        emit onAcc(id);
                    }
                }
            }
        }
    });
    QObject::connect(this, &Accounts::changeEnabled, [startButton, stopButton, pos, m_num] (int id, int num) {
        if(m_num == num) {
            if(pos == id) {
                startButton->setEnabled(!startButton->isEnabled());
                stopButton->setEnabled(!stopButton->isEnabled());
            }
        }
    });

    sLayout->addWidget(widget);
}

void Accounts::buttonsState(int pos, int num) {
    emit changeEnabled(pos, num);
}

void Accounts::checkState(int pos, int mode, int num) {
    emit checkStateButton(pos, mode, num);
}

