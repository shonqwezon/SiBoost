#ifndef UPLOADWINDOW_H
#define UPLOADWINDOW_H

#include <QWidget>
#include <QFileDialog>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QCheckBox>
#include <readdata.h>

namespace Ui {
class UploadWindow;
}

class UploadWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UploadWindow(QWidget *parent = nullptr);
    ~UploadWindow();

signals:
    void sendData(QJsonArray);

private slots:
    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::UploadWindow *ui;
    int mode = 0;
};

#endif // UPLOADWINDOW_H
