#include "settingsaddaccount.h"

#include "language/translate.h"
#include "wallet/events.h"
#include "crypto/signatures.h"
#include "wallet/check.h"
#include "storage/walletfile.h"
#include "wallet/rpc/wallethistory.h"
#include "pages/pc/populate.h"

#define s(s) _scale(s)

settingsaddaccount::settingsaddaccount() {

}

settingsaddaccount::~settingsaddaccount() {
    delete thisMdiArea;
}

void settingsaddaccount::init(QMdiArea *mdiArea) {
    this->parent = mdiArea;
    this->setParent(mdiArea);
    thisMdiArea = new QMdiArea(mdiArea);
    thisMdiArea->setVisible(false);

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

    newAccountButton = new QPushButton(thisMdiArea);
    recoverAccountButton = new QPushButton(thisMdiArea);

    accountNameLineEdit = new QLineEdit(thisMdiArea);
    privateKeyLineEdit = new QLineEdit(thisMdiArea);
    accountNameCheck = new QLabel(thisMdiArea);
    privateKeyCheck = new QLabel(thisMdiArea);

    passwordConfirmationLineEdit = new QLineEdit(thisMdiArea);

    cancelButton = new QPushButton(thisMdiArea);
    okButton = new QPushButton(thisMdiArea);


    newAccountButton->setCursor(Qt::PointingHandCursor);
    newAccountButton->setFlat(true);
    newAccountButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/transitions/green.png); border-radius: 3px; border: 1px solid #eee; color: #fff; ");
    newAccountButton->setVisible(false);
    connect(newAccountButton, SIGNAL(clicked()),this, SLOT(on_New_ButtonPressed()));

    recoverAccountButton->setCursor(Qt::PointingHandCursor);
    recoverAccountButton->setFlat(true);
    recoverAccountButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/transitions/green.png); border-radius: 3px; border: 1px solid #eee; color: #fff; ");
    recoverAccountButton->setVisible(false);
    connect(recoverAccountButton, SIGNAL(clicked()),this, SLOT(on_Recover_ButtonPressed()));

    accountNameLineEdit->setVisible(false);
    connect(accountNameLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_AccountNameLineEdit_Changed(const QString &)));
    accountNameLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");

    privateKeyLineEdit->setVisible(false);
    connect(privateKeyLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_PrivateKeyLineEdit_Changed(const QString &)));
    privateKeyLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");

    accountNameCheck->setScaledContents(true);
    accountNameCheck->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    accountNameCheck->setVisible(false);

    privateKeyCheck->setScaledContents(true);
    privateKeyCheck->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    privateKeyCheck->setVisible(false);

    passwordConfirmationLineEdit->setVisible(false);
    passwordConfirmationLineEdit->setEchoMode(QLineEdit::EchoMode::Password);
    passwordConfirmationLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");
    connect(passwordConfirmationLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_passwordConfirmationLineEdit_textChanged(const QString &)));

    cancelButton->setCursor(Qt::PointingHandCursor);
    cancelButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/transitions/green.png); border-radius: 6px; border: 1px solid #eee; color: #fff; ");
    cancelButton->setVisible(false);
    connect(cancelButton, SIGNAL(clicked()),this, SLOT(on_Cancel_ButtonPressed()));

    okButton->setCursor(Qt::PointingHandCursor);
    okButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/transitions/orange.png); border-radius: 6px; border: 1px solid #eee; color: #fff; ");
    okButton->setVisible(false);
    connect(okButton, SIGNAL(clicked()),this, SLOT(on_Ok_ButtonPressed()));

    refreshSize();
    refreshLanguage();
}

void settingsaddaccount::refreshFonts() {

    newAccountButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    recoverAccountButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    accountNameLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    privateKeyLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    passwordConfirmationLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    cancelButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    okButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
}

void settingsaddaccount::refreshSize() {
    newAccountButton->setGeometry(s(328), s(65), s(470), s(40));
    recoverAccountButton->setGeometry(s(328), s(115), s(470), s(40));

    accountNameLineEdit->setGeometry(s(328), s(65), s(470), s(40));
    accountNameCheck->setGeometry(s(800), s(65), s(40), s(40));
    privateKeyLineEdit->setGeometry(s(328), s(115), s(470), s(40));
    privateKeyCheck->setGeometry(s(800), s(115), s(40), s(40));

    passwordConfirmationLineEdit->setGeometry(s(328), s(165), s(470), s(40));

    cancelButton->setGeometry(s(328), s(215), s(470), s(40));
    okButton->setGeometry(s(328), s(265), s(470), s(40));

    refreshFonts();
}

void settingsaddaccount::refreshLanguage() {
    newAccountButton->setText(_tr("NEW ACCOUNT"));
    recoverAccountButton->setText(_tr("RECOVER ACCOUNT FROM PRIVATE KEY"));

    accountNameLineEdit->setPlaceholderText(_tr("Type the new account name"));
    privateKeyLineEdit->setPlaceholderText(_tr("Type the private key"));

    passwordConfirmationLineEdit->setPlaceholderText(_tr("Type the wallet password"));

    cancelButton->setText(_tr("CANCEL"));
    okButton->setText(_tr("OK"));

    refreshFonts();
}

settingsaddaccount::runMode_e settingsaddaccount::getState() {
    return currentMode;
}

void settingsaddaccount::setState(runMode_e state) {
    currentMode = state;
}

settingsaddaccount::return_e settingsaddaccount::run() {
    if(pastMode != currentMode) {
        newAccountButton->setVisible(false);
        recoverAccountButton->setVisible(false);
        accountNameLineEdit->setVisible(false);
        privateKeyLineEdit->setVisible(false);
        privateKeyCheck->setVisible(false);
        accountNameCheck->setVisible(false);
        passwordConfirmationLineEdit->setVisible(false);
        cancelButton->setVisible(false);
        okButton->setVisible(false);
        if(currentMode == runMode_e::CHOOSE) {
            choose = return_e::RETURN_NONE;
            newAccountButton->setVisible(true);
            recoverAccountButton->setVisible(true);
            thisMdiArea->setVisible(true);
            thisWindow->setVisible(true);
        } else if(currentMode == runMode_e::NEW) {
            choose = return_e::RETURN_NONE;
            newAccountButton->setVisible(false);
            recoverAccountButton->setVisible(false);


            accountNameLineEdit->clear();
            accountNameLineEdit->setVisible(true);
            passwordConfirmationLineEdit->clear();
            passwordConfirmationLineEdit->setVisible(true);
            privateKeyLineEdit->clear();
            privateKeyLineEdit->setVisible(true);
            privateKeyLineEdit->setReadOnly(true);
            privateKeyCheck->setVisible(true);
            accountNameCheck->setVisible(true);
            signatures::generatedAccount wallet = signatures::generateAccount();
            privateKeyLineEdit->setText(wallet.privateKey);
            cancelButton->setVisible(true);
            okButton->setVisible(true);
            thisMdiArea->setVisible(true);
            thisWindow->setVisible(true);
        } else if(currentMode == runMode_e::RECOVER) {
            choose = return_e::RETURN_NONE;
            newAccountButton->setVisible(false);
            recoverAccountButton->setVisible(false);

            accountNameLineEdit->clear();
            accountNameLineEdit->setVisible(true);
            passwordConfirmationLineEdit->clear();
            passwordConfirmationLineEdit->setVisible(true);
            privateKeyLineEdit->clear();
            privateKeyLineEdit->setVisible(true);
            privateKeyLineEdit->setReadOnly(false);
            privateKeyCheck->setVisible(true);
            accountNameCheck->setVisible(true);
            cancelButton->setVisible(true);
            okButton->setVisible(true);
            thisMdiArea->setVisible(true);
            thisWindow->setVisible(true);
        } else  if(currentMode == runMode_e::CANCEL) {
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
    return return_e::RETURN_NONE;
}

void settingsaddaccount::on_New_ButtonPressed() {
    currentMode = runMode_e::NEW;
}

void settingsaddaccount::on_Recover_ButtonPressed() {
    currentMode = runMode_e::RECOVER;
}

void settingsaddaccount::on_Cancel_ButtonPressed() {
    currentMode = runMode_e::CANCEL;
}

void settingsaddaccount::on_Ok_ButtonPressed() {
    if(!passwordConfirmationLineEdit->text().compare(events::getWalletUserPassword().second) && signatures::validatePrivateKey(privateKeyLineEdit->text()) && check::name(accountNameLineEdit->text())) {
        okButton->setText(_tr("WAIT"));
        okButton->repaint();
        cancelButton->setVisible(false);
        cancelButton->repaint();
        events::addAccountNameKeyList(QPair<QString, QString>(accountNameLineEdit->text(), privateKeyLineEdit->text()));
        wallethistory::addAccount(accountNameLineEdit->text(), 0);
        wallethistory::addAccount(accountNameLineEdit->text(), 1);
        events::setId(signatures::getAccountIdFromPrivateKey(privateKeyLineEdit->text()));
        QList<QPair<QString, QString>> list = events::getAccountNameIdList();
        QPair<QString, QString> pair = list[list.count() - 1];
        wallethistory::updateAccount(pair.first, pair.second);
        events::setWalletHistoryChanged();
        walletfile::save(events::getWalletUserPassword().first, events::getWalletUserPassword().second);
        populate::refreshAll();
        events::setSelectedNameKeyIndex(events::getAccountNameList().count() - 1);
        currentMode = runMode_e::OK;
        okButton->setText(_tr("OK"));
        cancelButton->setVisible(true);
    }
}

void settingsaddaccount::on_AccountNameLineEdit_Changed(const QString &text) {
    if(check::name(text)) {
        accountNameCheck->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/ok.png); border-radius: 1px; color: #eee; ");
    } else {
        accountNameCheck->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    }
}

void settingsaddaccount::on_PrivateKeyLineEdit_Changed(const QString &text) {
    if(signatures::validatePrivateKey(text)) {
        privateKeyCheck->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/ok.png); border-radius: 1px; color: #eee; ");
    } else {
        privateKeyCheck->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/warning.png); border-radius: 1px; color: #eee; ");
    }
}

void settingsaddaccount::on_passwordConfirmationLineEdit_textChanged(const QString &) {
    if(!passwordConfirmationLineEdit->text().compare(events::getWalletUserPassword().second)) {
        passwordConfirmationLineEdit->setStyleSheet("QLineEdit {   "
                                                        "color: #A0B9CE;"
                                                        "border-color: white;"
                                                        "background-color: white;"
                                                        "border: 1px solid #eee;"
                                                        "border-radius: 3px;"
                                                        ";}");
    } else {
        passwordConfirmationLineEdit->setStyleSheet("QLineEdit {   "
                                                        "color: #FF151F;"
                                                        "border-color: white;"
                                                        "background-color: white;"
                                                        "border: 1px solid #eee;"
                                                        "border-radius: 3px;"
                                                        ";}");
    }
}



