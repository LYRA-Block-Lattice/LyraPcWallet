#include <QException>

#include "mainwindow.h"

#include "keyrevealer.h"
#include "ui_keyrevealer.h"

#include "wallet/events.h"

keyrevealer::keyrevealer(int nr, bool immediate, bool persistent, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyrevealer) {
    if(parent) {
        QRect hostRect = MainWindow::getParent()->geometry();
        this->move(hostRect.center() - this->rect().center());
    }
    ui->setupUi(this);
    this->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    this->nr = nr;
    this->immediate = immediate;
    this->persistent = persistent;

    if(MainWindow::getPassword().length() != 0) {
        if(!events::getWalletUserPassword().second.compare(MainWindow::getPassword())) {
            qDebug() << "KEYREVEALER 1: " << "Nonimediate OK";
            pKey = events::getWalletNameKeyList().at(nr).second;
            this->hide();
        } else {
            qDebug() << "KEYREVEALER 2: " << "Nonimediate unmatched";
            pKey = "";
        }
    }
}

keyrevealer::~keyrevealer() {
    delete ui;
}

void keyrevealer::on_buttonBox_accepted(){
    if(persistent && (MainWindow::getPassword().length() == 0 || immediate)) {
        if(!events::getWalletUserPassword().second.compare(ui->lineEdit->text())) {
            qDebug() << "KEYREVEALER 3: " << "Persistent OK";
            MainWindow::setPassword(ui->lineEdit->text(), persistent);
            pKey = events::getWalletNameKeyList().at(nr).second;
        } else {
            qDebug() << "KEYREVEALER 4: " << "Persistent unmatched";
            pKey = "";
        }
        return;
    } else {
        if(!events::getWalletUserPassword().second.compare(ui->lineEdit->text())) {
            qDebug() << "KEYREVEALER 5: " << "Nonpersistent OK";
            pKey = events::getWalletNameKeyList().at(nr).second;
        } else {
            qDebug() << "KEYREVEALER 6: " << "Nonpersistent unmatched";
            pKey = "";
        }
        return;
    }
    qDebug() << "KEYREVEALER 7: " << "Not meet";
}

void keyrevealer::on_buttonBox_rejected(){
    result = keyrevealer::RESULT_REJECTED;
}

QString keyrevealer::getKey() {
    if(!pKey.length()) {
        exec();
    }
    QString tmp = pKey;
    pKey = "";
    return tmp;
}
