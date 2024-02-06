#include "mainwindow.h"
#include "uploadwindow.h"
#include <QApplication>
#include <QFile>
#include <QJsonArray>
#include <QMessageBox>
#include <readdata.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Siboost Inc.");
    QCoreApplication::setApplicationName("Siboost");

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QCoreApplication::applicationName());

    UploadWindow d;
    QObject::connect(&d, &UploadWindow::sendData, [&w](QJsonArray object){
        w.setUser(object);
        w.show();
    });

    QString path = QCoreApplication::applicationDirPath()+"/data.txt";

    if(QFile::exists(path)) {
        QJsonArray object = ReadData::read(path);
        if(!object.empty()) {
            w.setUser(object);
            w.show();
        }
        else {
            QFile::remove(path);
            QMessageBox::critical(0, "Ошибка!", "Невозможно прочитать данные!");
            d.show();
        }
    }
    else {
        d.show();
    }
    return a.exec();
}
