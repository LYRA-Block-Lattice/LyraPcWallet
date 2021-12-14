#include <QException>

#include "mainwindow.h"
#include "keyrevealer.h"
#include "ui_keyrevealer.h"

#include "wallet/events.h"
#include "pages/pc/main/mainboard.h"

keyrevealer::keyrevealer(int nr, bool immediate, bool persistent, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyrevealer) {
    ui->setupUi(this);

    QRect hostRect = MainWindow::getParent()->geometry();
    this->move(hostRect.center() - this->rect().center());
    QPushButton *cancelButton = ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel);
    cancelButton->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 " COLOR_GREY_BRIGHT ", stop: 1 " COLOR_GREY_DARK "); border-radius: 10px; color: #eee; min-width: 70px; min-height: 25px;");
    cancelButton->setVisible(false);
    QPushButton *okButton = ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok);
    okButton->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 " COLOR_GREY_BRIGHT ", stop: 1 " COLOR_GREY_DARK "); border-radius: 10px; color: #eee; min-width: 70px; min-height: 25px;");
    okButton->setVisible(false);
    QTimer::singleShot(0, this, [this] {
        ui->lineEdit->setFocus();
    });
    this->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->nr = nr;
    this->immediate = immediate;
    this->persistent = persistent;

    if(MainWindow::getPassword().length() != 0 && !immediate) {
        if(!events::getWalletUserPassword().second.compare(MainWindow::getPassword())) {
            qDebug() << "KEYREVEALER 1: " << "Nonimediate OK";
            pKey = events::getAccountKeyNoP(nr);
            this->hide();
        } else {
            qDebug() << "KEYREVEALER 2: " << "Nonimediate unmatched";
            pKey = "";
        }
    } else {
        if(!blurEffectCentralWidget) {
            blurEffectCentralWidget = new QGraphicsBlurEffect();
        }
        blurEffectCentralWidget->setBlurRadius(5.0);
        MainWindow::getParent()->setGraphicsEffect(blurEffectCentralWidget);
        MainWindow::refreshParentWindow();
    }
}

keyrevealer::~keyrevealer() {
    if(!blurEffectCentralWidget) {
        blurEffectCentralWidget = new QGraphicsBlurEffect();
    }
    blurEffectCentralWidget->setBlurRadius(0.0);
    MainWindow::getParent()->setGraphicsEffect(blurEffectCentralWidget);
    MainWindow::refreshParentWindow();
    delete ui;
}

void keyrevealer::on_buttonBox_accepted(){
    if(persistent && (MainWindow::getPassword().length() == 0 || immediate)) {
        if(!events::getWalletUserPassword().second.compare(ui->lineEdit->text())) {
            qDebug() << "KEYREVEALER 3: " << "Persistent OK";
            MainWindow::setPassword(ui->lineEdit->text(), persistent);
            pKey = events::getAccountKeyNoP(nr);
        } else {
            qDebug() << "KEYREVEALER 4: " << "Persistent unmatched";
            pKey = "";
        }
        return;
    } else {
        if(!events::getWalletUserPassword().second.compare(ui->lineEdit->text())) {
            qDebug() << "KEYREVEALER 5: " << "Nonpersistent OK";
            pKey = events::getAccountKeyNoP(nr);
        } else {
            qDebug() << "KEYREVEALER 6: " << "Nonpersistent unmatched";
            pKey = "";
        }
        return;
    }
    qDebug() << "KEYREVEALER 7: " << "Not meet";
    if(!blurEffectCentralWidget) {
        blurEffectCentralWidget = new QGraphicsBlurEffect();
    }
    blurEffectCentralWidget->setBlurRadius(0.0);
    MainWindow::getParent()->setGraphicsEffect(blurEffectCentralWidget);
    MainWindow::refreshParentWindow();
}

void keyrevealer::on_buttonBox_rejected(){
    if(!blurEffectCentralWidget) {
        blurEffectCentralWidget = new QGraphicsBlurEffect();
    }
    blurEffectCentralWidget->setBlurRadius(0.0);
    MainWindow::getParent()->setGraphicsEffect(blurEffectCentralWidget);
    MainWindow::refreshParentWindow();
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


void keyrevealer::on_lineEdit_returnPressed()
{
    on_buttonBox_accepted();
    this->hide();
}

