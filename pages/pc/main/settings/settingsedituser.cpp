#include "settingsedituser.h"

#include "language/translate.h"
#include "wallet/events.h"
#include "wallet/check.h"
#include "storage/walletfile.h"

#define s(s) _scale(s)

settingsedituser::settingsedituser() {

}

settingsedituser::~settingsedituser() {
    delete thisMdiArea;
}

void settingsedituser::init(QMdiArea *mdiArea) {
    this->parent = mdiArea;
    this->setParent(mdiArea);
    thisMdiArea = new QMdiArea(mdiArea);
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

    userLineEdit = new QLineEdit(thisMdiArea);
    accountNameCheck = new QLabel(thisMdiArea);
    oldPasswordLineEdit = new QLineEdit(thisMdiArea);
    newPassword1LineEdit = new QLineEdit(thisMdiArea);
    newPassword2LineEdit = new QLineEdit(thisMdiArea);
    cancelButton = new QPushButton(thisMdiArea);
    okButton = new QPushButton(thisMdiArea);

    userLineEdit->setAttribute(Qt::WA_TranslucentBackground, true);
    userLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");
    connect(userLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_UserNameLineEdit_Changed(const QString &)));

    accountNameCheck->setScaledContents(true);
    accountNameCheck->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");

    oldPasswordLineEdit->setEchoMode(QLineEdit::EchoMode::Password);
    oldPasswordLineEdit->setAttribute(Qt::WA_TranslucentBackground, true);
    oldPasswordLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");
    connect(oldPasswordLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_oldPasswordLineEdit_textChanged(const QString &)));

    newPassword1LineEdit->setEchoMode(QLineEdit::EchoMode::Password);
    newPassword1LineEdit->setAttribute(Qt::WA_TranslucentBackground, true);
    newPassword1LineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");
    connect(newPassword1LineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_newPassword1LineEdit_textChanged(const QString &)));

    newPassword2LineEdit->setEchoMode(QLineEdit::EchoMode::Password);
    newPassword2LineEdit->setAttribute(Qt::WA_TranslucentBackground, true);
    newPassword2LineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");
    connect(newPassword2LineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_newPassword2LineEdit_textChanged(const QString &)));

    cancelButton->setCursor(Qt::PointingHandCursor);
    cancelButton->setFlat(true);
    cancelButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/transitions/green.png); border-radius: 3px; border: 1px solid #eee; color: #fff; ");
    connect(cancelButton, SIGNAL(clicked()),this, SLOT(on_Cancel_ButtonPressed()));

    okButton->setCursor(Qt::PointingHandCursor);
    okButton->setFlat(true);
    okButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/transitions/orange.png); border-radius: 3px; border: 1px solid #eee; color: #fff; ");
    connect(okButton, SIGNAL(clicked()),this, SLOT(on_Ok_ButtonPressed()));

    refreshSize();
    refreshLanguage();
}

void settingsedituser::refreshFonts() {
    userLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    oldPasswordLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    newPassword1LineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    newPassword2LineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    cancelButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    okButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

}

void settingsedituser::refreshSize() {
    userLineEdit->setGeometry(s(328), s(65), s(470), s(40));
    accountNameCheck->setGeometry(s(800), s(65), s(40), s(40));
    oldPasswordLineEdit->setGeometry(s(328), s(115), s(470), s(40));
    newPassword1LineEdit->setGeometry(s(328), s(165), s(470), s(40));
    newPassword2LineEdit->setGeometry(s(328), s(215), s(470), s(40));

    cancelButton->setGeometry(s(328), s(265), s(470), s(40));
    okButton->setGeometry(s(328), s(315), s(470), s(40));

    refreshFonts();
}

void settingsedituser::refreshLanguage() {
    userLineEdit->setPlaceholderText(_tr("Type the new username for this wallet"));
    oldPasswordLineEdit->setPlaceholderText(_tr("Type the wallet password"));
    newPassword1LineEdit->setPlaceholderText(_tr("Type the new wallet password"));
    newPassword2LineEdit->setPlaceholderText(_tr("Retype the new wallet password"));

    cancelButton->setText(_tr("CANCEL"));
    okButton->setText(_tr("OK"));

    refreshFonts();
}

settingsedituser::runMode_e settingsedituser::getState() {
    return currentMode;
}

void settingsedituser::setState(runMode_e state) {
    currentMode = state;
}

settingsedituser::return_e settingsedituser::run() {
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
            choose = return_e::RETURN_NONE;
            userLineEdit->clear();
            oldPasswordLineEdit->clear();
            newPassword1LineEdit->clear();
            newPassword2LineEdit->clear();
            thisMdiArea->setVisible(true);
            thisWindow->setVisible(true);
        } else if(currentMode == runMode_e::CANCEL) {
            thisMdiArea->setVisible(false);
            thisWindow->setVisible(false);
            currentMode = runMode_e::NONE;
            return return_e::RETURN_CANCEL;
        } else  if(currentMode == runMode_e::OK) {
            thisMdiArea->setVisible(false);
            thisWindow->setVisible(false);
            currentMode = runMode_e::NONE;
            return return_e::RETURN_OK;
        } else if(currentMode == runMode_e::NONE) {
            thisMdiArea->setVisible(false);
            thisWindow->setVisible(false);
        }
        pastMode = currentMode;
    }
    return return_e::RETURN_NONE;
}

void settingsedituser::on_UserNameLineEdit_Changed(const QString &text) {
    if(check::name(text)) {
        accountNameCheck->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/ok.png); border-radius: 1px; color: #eee; ");
    } else {
        accountNameCheck->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    }
}

void settingsedituser::on_Cancel_ButtonPressed() {
    currentMode = runMode_e::CANCEL;
}

void settingsedituser::on_Ok_ButtonPressed() {
    if(!oldPasswordLineEdit->text().compare(events::getWalletUserPassword().second)) {
        bool editUser = false;
        bool editPassword = false;
        if(userLineEdit->text().length() > 0 && check::name(userLineEdit->text())) {
            editUser = true;
        }
        if(check::password(newPassword1LineEdit->text(), newPassword2LineEdit->text())) {
            editPassword = true;
        }
        if(editUser && editPassword) {
            events::setWalletHistoryChanged();
            walletfile::save(userLineEdit->text(), newPassword1LineEdit->text());
            events::setWalletUserPassword({userLineEdit->text(), newPassword1LineEdit->text()});
        } else if(editUser) {
            events::setWalletHistoryChanged();
            walletfile::save(userLineEdit->text(), events::getWalletUserPassword().second);
            events::setWalletUserPassword(QPair<QString, QString>({userLineEdit->text(), events::getWalletUserPassword().second}));
        } else if(editPassword) {
            events::setWalletUserPassword({events::getWalletUserPassword().first, newPassword1LineEdit->text()});
            events::setWalletHistoryChanged();
            walletfile::save(events::getWalletUserPassword().first, events::getWalletUserPassword().second);
        }
    }
    currentMode = runMode_e::OK;
}

void settingsedituser::on_oldPasswordLineEdit_textChanged(const QString &) {
    if(!oldPasswordLineEdit->text().compare(events::getWalletUserPassword().second)) {
        oldPasswordLineEdit->setStyleSheet("QLineEdit {   "
                                                        "color: #A0B9CE;"
                                                        "border-color: white;"
                                                        "background-color: white;"
                                                        "border: 1px solid #eee;"
                                                        "border-radius: 3px;"
                                                        ";}");
    } else {
        oldPasswordLineEdit->setStyleSheet("QLineEdit {   "
                                                        "color: #FF151F;"
                                                        "border-color: white;"
                                                        "background-color: white;"
                                                        "border: 1px solid #eee;"
                                                        "border-radius: 3px;"
                                                        ";}");
    }
}

void settingsedituser::on_newPassword1LineEdit_textChanged(const QString &arg1) {
    if(!check::password(arg1)) {
        newPassword1LineEdit->setStyleSheet("QLineEdit {   "
                                                        "color: #FF151F;"
                                                        "border-color: white;"
                                                        "background-color: white;"
                                                        "border: 1px solid #eee;"
                                                        "border-radius: 3px;"
                                                        ";}");
    } else {
        newPassword1LineEdit->setStyleSheet("QLineEdit {   "
                                                        "color: #A0B9CE;"
                                                        "border-color: white;"
                                                        "background-color: white;"
                                                        "border: 1px solid #eee;"
                                                        "border-radius: 3px;"
                                                        ";}");
    }
}

void settingsedituser::on_newPassword2LineEdit_textChanged(const QString &arg1) {
    if(!check::password(newPassword1LineEdit->text(), arg1)) {
        newPassword2LineEdit->setStyleSheet("QLineEdit {   "
                                                        "color: #FF151F;"
                                                        "border-color: white;"
                                                        "background-color: white;"
                                                        "border: 1px solid #eee;"
                                                        "border-radius: 3px;"
                                                        ";}");
    } else {
        newPassword2LineEdit->setStyleSheet("QLineEdit {   "
                                                        "color: #A0B9CE;"
                                                        "border-color: white;"
                                                        "background-color: white;"
                                                        "border: 1px solid #eee;"
                                                        "border-radius: 3px;"
                                                        ";}");
    }
}


