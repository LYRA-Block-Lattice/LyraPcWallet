#include <QApplication>

#include "mainwindow.h"
#include "wallet/events.h"

#include "settingsshowprivkey.h"
#include "language/translate.h"

#define s(s) _scale(s)

settingsshowprivkey::settingsshowprivkey() {

}

settingsshowprivkey::~settingsshowprivkey() {
    delete thisMdiArea;
}

void settingsshowprivkey::init(QMdiArea *mdiArea) {
    this->parent = mdiArea;
    this->setParent(mdiArea);
    thisMdiArea = new QMdiArea(mdiArea);
    //thisMdiArea.setParent(mdiArea);
    thisMdiArea->setVisible(false);
    //this->walletName = walletName;

    thisWidget = new QWidget(thisMdiArea);
    thisWidget->show();
    thisWindow = new QMdiSubWindow(thisMdiArea);
    thisWindow->setWidget(thisWidget);
    thisWindow->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    thisWindow->setWindowFlag(Qt::FramelessWindowHint, true);
    thisWindow->setGeometry(0, 0, thisMdiArea->width(), thisMdiArea->height());
    thisWindow->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/settings/whiteBack.png)");
    thisWindow->setVisible(false);

    titleName = new QLabel(thisMdiArea);

    privateKey = new QLineEdit(thisMdiArea);

    okButton = new QPushButton(thisMdiArea);

    titleName->setStyleSheet("color: #333;");
    titleName->setAlignment(Qt::AlignCenter);
    titleName->setAttribute(Qt::WA_TranslucentBackground, true);

    privateKey->setAttribute(Qt::WA_TranslucentBackground, true);
    privateKey->setAlignment(Qt::AlignCenter);
    privateKey->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");
    privateKey->setReadOnly(true);

    okButton->setCursor(Qt::PointingHandCursor);
    okButton->setFlat(true);
    okButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/transitions/green.png); border-radius: 3px; border: 1px solid #eee; color: #fff; ");
    connect(okButton, SIGNAL(clicked()),this, SLOT(on_Ok_ButtonPressed()));

    refreshSize();
    refreshLanguage();
}

void settingsshowprivkey::refreshFonts() {
    titleName->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.0)));

    privateKey->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    okButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
}

void settingsshowprivkey::refreshSize() {
    titleName->setGeometry(s(328), s(10), s(470), s(40));

    privateKey->setGeometry(s(328), s(65), s(470), s(40));

    okButton->setGeometry(s(328), s(115), s(470), s(40));
    refreshFonts();
}

void settingsshowprivkey::refreshLanguage() {
    okButton->setText(_tr("OK"));

    refreshFonts();
}

settingsshowprivkey::runMode_e settingsshowprivkey::getState() {
    return currentMode;
}

void settingsshowprivkey::setState(runMode_e state, int keyNr) {
    currentMode = state;
    if(keyNr >= 0) {
        keyrevealer keyRevealer(keyNr, true, false, parent);
        QString key = keyRevealer.getKey();
        QList<QPair<QString, QString>> tmp = events::getWalletNameKeyList();
        if(tmp.count() > keyNr) {
            if(key.length()) {
                titleName->setText(tmp[keyNr].first);
                privateKey->setText(key);
            } else {
                currentMode = runMode_e::NONE;
            }
        }
    }
    qDebug() << state;
}

void settingsshowprivkey::run() {
    if(pastMode != currentMode) {
        if(pastScale != events::getScale()) {
            pastScale = events::getScale();
            thisMdiArea->setGeometry(0, 0, parent->width(), parent->height());
            thisWindow->setGeometry(0, 0, thisMdiArea->width(), thisMdiArea->height());
            refreshSize();
        }
        if(pastLanguage.compare(translate::getCurrentLang())) {
            pastLanguage = translate::getCurrentLang();
            refreshLanguage();
        }
        if(currentMode == runMode_e::RUN) {
            thisMdiArea->setVisible(true);
            thisWindow->setVisible(true);
        } else if(currentMode == runMode_e::NONE) {
            privateKey->clear();
            thisMdiArea->setVisible(false);
            thisWindow->setVisible(false);
        }
        pastMode = currentMode;
    }
}

void settingsshowprivkey::on_Ok_ButtonPressed() {
    currentMode = runMode_e::NONE;
}




