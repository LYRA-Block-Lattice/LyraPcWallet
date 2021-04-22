#include "settingsdeleteaccount.h"

#include "language/translate.h"
#include "wallet/events.h"
#include "crypto/signatures.h"
#include "wallet/rpc/wallethistory.h"
#include "storage/walletfile.h"

#define s(s) _scale(s)

settingsdeleteaccount::settingsdeleteaccount() {

}

settingsdeleteaccount::~settingsdeleteaccount() {
    delete thisMdiArea;
}

void settingsdeleteaccount::init(QMdiArea *mdiArea) {
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

    passwordConfirmationLineEdit = new QLineEdit(thisMdiArea);
    cancelButton = new QPushButton(thisMdiArea);
    okButton = new QPushButton(thisMdiArea);

    passwordConfirmationLineEdit->setEchoMode(QLineEdit::EchoMode::Password);
    passwordConfirmationLineEdit->setStyleSheet("QLineEdit {   "
                                                    "color: #555;"
                                                    "border-color: white;"
                                                    "background-color: white;"
                                                    "border: 1px solid #eee;"
                                                    "border-radius: 3px;"
                                                    ";}");

    cancelButton->setCursor(Qt::PointingHandCursor);
    cancelButton->setFlat(true);
    cancelButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/transitions/green.png); border-radius: 3px; border: 1px solid #eee; color: #fff; ");
    connect(cancelButton, SIGNAL(clicked()),this, SLOT(on_Cancel_ButtonPressed()));

    okButton->setCursor(Qt::PointingHandCursor);
    okButton->setFlat(true);
    okButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/transitions/red.png); border-radius: 3px; border: 1px solid #eee; color: #fff; ");
    connect(okButton, SIGNAL(clicked()),this, SLOT(on_Ok_ButtonPressed()));

    refreshSize();
    refreshLanguage();
}

void settingsdeleteaccount::refreshFonts() {
    passwordConfirmationLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));

    cancelButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    okButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
}

void settingsdeleteaccount::refreshSize() {
    passwordConfirmationLineEdit->setGeometry(s(328), s(65), s(470), s(40));

    cancelButton->setGeometry(s(328), s(115), s(470), s(40));
    okButton->setGeometry(s(328), s(165), s(470), s(40));

    refreshFonts();
}

void settingsdeleteaccount::refreshLanguage() {
    passwordConfirmationLineEdit->setPlaceholderText(_tr("Type the wallet password"));

    cancelButton->setText(_tr("CANCEL"));
    okButton->setText(_tr("OK"));

    refreshFonts();
}

settingsdeleteaccount::runMode_e settingsdeleteaccount::getState() {
    return currentMode;
}

void settingsdeleteaccount::setState(runMode_e state, QString walletName) {
    this->walletName = walletName;
    currentMode = state;
}

settingsdeleteaccount::return_e settingsdeleteaccount::run() {
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
            passwordConfirmationLineEdit->clear();
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

void settingsdeleteaccount::on_Cancel_ButtonPressed() {
    currentMode = runMode_e::CANCEL;

}

void settingsdeleteaccount::on_Ok_ButtonPressed() {
    if(!passwordConfirmationLineEdit->text().compare(events::getWalletUserPassword().second)) {
        events::removeWalletNameKeyList(walletName);
        wallethistory::removeWallet(walletName);
        events::setWalletHistoryChanged();
        walletfile::save(events::getWalletUserPassword().first, events::getWalletUserPassword().second);
        if(events::getWalletNameList().count() != 0) {
            events::setSelectedNameKeyIndex(events::getWalletNameList().count() - 1);
        } else {
            events::setId("");
        }
        currentMode = runMode_e::OK;
    }
}

