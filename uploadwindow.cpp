#include "uploadwindow.h"
#include "ui_uploadwindow.h"

UploadWindow::UploadWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UploadWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QCoreApplication::applicationName());
}

UploadWindow::~UploadWindow()
{
    delete ui;
}

void UploadWindow::on_pushButton_clicked()
{
    QStringList list = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
    QString str = QFileDialog::getOpenFileName(0, "Открыть", list.first(), "*.txt");
    if(!str.isEmpty()) {
        QJsonArray object = ReadData::read(str);
        if(!object.empty()) {
            if(mode != 0) if(!QFile::copy(str, QCoreApplication::applicationDirPath()+"/data.txt")) QMessageBox::critical(0, "Ошибка!", "Невозможно сохранить данные!");
            emit sendData(object);
            close();
        }
        else {
            QMessageBox::critical(0, "Ошибка!", "Возможные причины ошибки:\n   *файл невозможно\n   *файл пустой\n   *аккаунты не соответствует формату записи\n    (Логин Пароль)");
        }
    }
}

void UploadWindow::on_checkBox_stateChanged(int arg1)
{
    mode = arg1;
}
