#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMap>
#include <params.h>
#include <QDialog>
#include <QLabel>
#include <QMessageBox>


class Accounts : public QObject {
    Q_OBJECT

public:
    Accounts(Params*);
    ~Accounts();
    QVBoxLayout* getCLayout();
    QVBoxLayout* getSLayout();
    void changeAccs(QVBoxLayout*, int, QString, QString);
    void addMainWidgets(QPushButton*);
    void addNewAcc();
    void changeAcc(int);
    void mainAccs(QVBoxLayout*, int, QString, QString, int);
    void buttonsState(int, int);
    void checkState(int, int, int);

signals:
    void sendRepaintChange(QWidget*);
    void sendCreateScroll();
    void onAcc(int);
    void offAcc(int);
    void changeEnabled(int,int);
    void checkStateButton(int, int, int);

private slots:
    void deleteAcc(int);

private:
    QVBoxLayout* cLayout;
    QVBoxLayout* sLayout;
    QLayoutItem* mItem;
    QMap<int, QWidget*> list;
    Params* params;
};

#endif // ACCOUNTS_H
