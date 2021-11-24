#include "login.h"

#include <QtGlobal>
#include <QApplication>
#include <QEvent>
#include <QTimer>
#include <QEventLoop>
#include <QRegularExpression>
#include <QPair>
#include <QThread>

#include "language/translate.h"

#include "wallet/check.h"
#include "wallet/events.h"
#include "storage/walletfile.h"
#include "wallet/rpc/wallethistory.h"
#include "crypto/signatures.h"

#define COMMON_STYLE                                "background-repeat: no-repeat; background-position: right; border: 1px solid #eee; border-radius: 5px; "
#define COMMON_PRIVATE_KEY_LINE_EDIT_PICTURE        "background-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_PrivateKeyLineEdit_Light.png); "
#define COMMON_USER_LINE_EDIT_PICTURE               "background-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_NameLineEdit_Light.png); "
#define COMMON_PASS_LINE_EDIT_PICTURE               "background-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_PasswordLineEdit_Light.png); "
#define COMMON_GREEN_BUTTON                         "border-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_ButtonGreen_Light.png); border-radius: 5px; color: white; "
#define COMMON_RED_BUTTON                           "border-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_ButtonRed_Light.png); border-radius: 5px; color: white; "
#define COMMON_DEFAULT_COLOR                        " color: #777; "
#define COMMON_ERROR_COLOR                          " color: red; "

#define s(s) _scale(s)

login::login(QMdiArea *mdiArea, QWidget *parent) {
    //this->setParent(parent);
    this->parent = parent;
    this->mdiArea = mdiArea;
    widget = new QWidget(mdiArea);
    widget->show();
    window = new QMdiSubWindow(mdiArea);
    //window->installEventFilter(this);
    setParent(mdiArea);
    window->setWidget(widget);
    window->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    window->setWindowFlag(Qt::FramelessWindowHint, true);
    widget->setGeometry(0, 0, mdiArea->width(), mdiArea->height());
    window->setGeometry(0, 0, mdiArea->width(), mdiArea->height());

    window->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_Light.png)");


    titleText = new QLabel(window);
    titleText->setVisible(false);

    /*createNewWallet = new QPushButton(window);
    createNewWallet->setVisible(false);
    connect(createNewWallet, SIGNAL(clicked()),this, SLOT(on_createNewWallet_pushed()));

    openWallet = new QPushButton(window);
    openWallet->setVisible(false);
    connect(openWallet, SIGNAL(clicked()),this, SLOT(on_openWallet_pushed()));

    restoreWallet = new QPushButton(window);
    restoreWallet->setVisible(false);
    connect(restoreWallet, SIGNAL(clicked()),this, SLOT(on_restoreWallet_pushed()));*/

    privKeyLineEdit = new QLineEdit(mdiArea);
    privKeyLineEdit->setVisible(false);
    connect(privKeyLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_privateKeyLineEdit_textChanged(const QString &)));

    nameLineEdit = new QLineEdit(mdiArea);
    nameLineEdit->setVisible(false);
    connect(nameLineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_nameNameLineEdit_textChanged(const QString &)));

    pass1LineEdit = new QLineEdit(mdiArea);
    pass1LineEdit->setVisible(false);
    connect(pass1LineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_pass1LineEdit_textChanged(const QString &)));
    connect(pass1LineEdit, &QLineEdit::returnPressed, this, &login::on_pass1EnterPushed);

    pass2LineEdit = new QLineEdit(mdiArea);
    pass2LineEdit->setVisible(false);
    connect(pass2LineEdit, SIGNAL(textChanged(const QString &)),this, SLOT(on_pass2LineEdit_textChanged(const QString &)));

    createNewUser = new QPushButton(mdiArea);
    createNewUser->setVisible(false);
    connect(createNewUser, SIGNAL(clicked()),this, SLOT(on_createNewWallet_pushed()));

    loginManager = new QPushButton(mdiArea);
    loginManager->setVisible(false);
    connect(loginManager, SIGNAL(clicked()),this, SLOT(on_login_pushed()));

    getStarted = new QPushButton(mdiArea);
    getStarted->setVisible(false);
    connect(getStarted, SIGNAL(clicked()),this, SLOT(on_getStarted_pushed()));

    back = new QPushButton(mdiArea);
    back->setVisible(false);
    connect(back, SIGNAL(clicked()),this, SLOT(on_back_pushed()));

    info = new QPushButton(mdiArea);
    info->setVisible(false);
    connect(info, SIGNAL(clicked()),this, SLOT(on_info_pushed()));

    infoMessage = new QLabel(mdiArea);
    infoMessage->setVisible(false);
    //loginManagerUser = new QLabel(window);
    //loginManagerUser->setVisible(false);
    lyraInc = new QLabel(mdiArea);
    lyraInc->setVisible(false);
}

login::~login() {

}

void login::setVars() {
}

login::command_e login::getCommand() {
    command_e cmd = command;
    command = command_e::CMD_NONE;
    return cmd;
}

void login::setState(state_e state) {
    currentState = state;
}

void login::run() {
    if(pastState != currentState || pastScale != events::getScale()  || pastLanguage.compare(translate::getCurrentLang()) || rfshCnt != 0) {
        if(rfshCnt > 0) {
            rfshCnt--;
        }
        infoSwitch = false;
        if(pastState != currentState) {
            titleText->setVisible(false);
            //createNewWallet.setVisible(false);
            //openWallet.setVisible(false);
            //restoreWallet.setVisible(false);
            privKeyLineEdit->setVisible(false);
            nameLineEdit->setVisible(false);
            pass1LineEdit->setVisible(false);
            pass2LineEdit->setVisible(false);
            createNewUser->setVisible(false);
            loginManager->setVisible(false);
            getStarted->setVisible(false);
            back->setVisible(false);
            info->setVisible(false);
            infoMessage->setVisible(false);
            //loginManagerUser->setVisible(false);
            lyraInc->setVisible(false);
        }

        if(pastState != currentState) {
            privKeyLineEdit->setText("");
            nameLineEdit->setText("");
            pass1LineEdit->setText("");
            pass2LineEdit->setText("");
        }

        //this->parent->repaint();
        if(!infoSwitch) {
            infoMessage->setVisible(false);
            window->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_Light.png)");
        } else {
            infoMessage->setVisible(true);
            window->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_Info_Light.png)");
        }
        if(currentState == STATE_NONE)
            window->setVisible(false);
        else
            window->setVisible(true);

        pastLanguage = translate::getCurrentLang();

        if(pastScale != events::getScale()) {
            rfshCnt = 1;
        }

        pastScale = events::getScale();

        if(currentState == STATE_LOGIN_PAGE) {
            /*createNewWallet.setGeometry(s(391), s(143), s(428), s(31));
            createNewWallet.setStyleSheet(COMMON_GREEN_BUTTON);
            createNewWallet.setText(_tr("CREATE NEW WALLET"));
            createNewWallet.setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            createNewWallet.setFlat(true);
            createNewWallet.setCursor(Qt::PointingHandCursor);
            createNewWallet.setVisible(true);

            openWallet.setGeometry(s(391), s(185), s(428), s(31));
            openWallet.setStyleSheet(COMMON_GREEN_BUTTON);
            openWallet.setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            openWallet.setText(_tr("OPEN WALLET"));
            openWallet.setFlat(true);
            openWallet.setCursor(Qt::PointingHandCursor);
            openWallet.setVisible(true);

            restoreWallet.setGeometry(s(391), s(227), s(428), s(31));
            restoreWallet.setStyleSheet(COMMON_GREEN_BUTTON);
            restoreWallet.setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            restoreWallet.setText(_tr("RESTORE WALLET"));
            restoreWallet.setFlat(true);
            restoreWallet.setCursor(Qt::PointingHandCursor);
            restoreWallet.setVisible(true);

            loginManagerUser.setText(_tr("or login wallet manager"));
            loginManagerUser.setStyleSheet(COMMON_STYLE COMMON_DEFAULT_COLOR);
            loginManagerUser.setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            loginManagerUser.setGeometry(s(391), s(267), s(428), s(20));
            loginManagerUser.setFont(QFont(translate::getCurrentFontLight(), s(9)));
            loginManagerUser.setAlignment(Qt::AlignCenter);
            loginManagerUser.setAttribute(Qt::WA_TranslucentBackground, true);
            loginManagerUser.setVisible(true);*/

            nameLineEdit->setGeometry(s(790), s(300), s(430), s(62));
            nameLineEdit->setPlaceholderText(_tr("Username"));
            nameLineEdit->setStyleSheet(COMMON_USER_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
            nameLineEdit->setTextMargins(s(5), 0, s(24), 0);
            nameLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1)));
            nameLineEdit->setVisible(true);

            pass1LineEdit->setGeometry(s(790), s(390), s(430), s(62));
            pass1LineEdit->setPlaceholderText(_tr("Password"));
            pass1LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
            pass1LineEdit->setTextMargins(s(5), 0, s(24), 0);
            pass1LineEdit->setEchoMode(QLineEdit::Password);
            pass1LineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1)));
            pass1LineEdit->setVisible(true);

            loginManager->setGeometry(s(790), s(480), s(430), s(62));
            loginManager->setStyleSheet(COMMON_GREEN_BUTTON);
            loginManager->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1)));
            loginManager->setText(_tr("LOG IN"));
            loginManager->setFlat(true);
            loginManager->setCursor(Qt::PointingHandCursor);
            loginManager->setVisible(true);


            getStarted->setGeometry(s(720), s(570), s(570), s(28));
            getStarted->setText(_tr("Don't Have an WALLET manager USER?") + " " + _tr("Get started!"));
            //restoreWallet.setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight()));
            getStarted->setStyleSheet("border-radius: 2px; " COMMON_DEFAULT_COLOR);
            getStarted->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1)));
            getStarted->setFlat(true);
            //getStarted.setStyleSheet("background-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_ButtonGetStarted_Light.png); border-radius: 2px; ");
            getStarted->setCursor(Qt::PointingHandCursor);
            getStarted->setVisible(true);

            info->setGeometry(s(578), s(906), s(28), s(28));
            info->setFlat(true);
            info->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_ButtonExclamation_Light.png); border-radius: 7px");
            info->setCursor(Qt::PointingHandCursor);
            info->setVisible(true);

            infoMessage->setGeometry(s(100), s(750), s(428), s(140));
            infoMessage->setText(_tr("LOG IN INFO"));
            infoMessage->setStyleSheet("color : white; ");
            infoMessage->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1)));
            infoMessage->setAttribute(Qt::WA_TranslucentBackground, true);
            infoMessage->setWordWrap(true);
            infoMessage->setLayoutDirection(Qt::LayoutDirection::LeftToRight);

        } else if(currentState == STATE_CREATE_MANAGER) {
            titleText->setGeometry(s(790), s(300), s(430), s(62));
            titleText->setText(_tr("CREATE NEW MANAGER USER"));
            titleText->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1)));
            titleText->setAlignment(Qt::AlignCenter);
            titleText->setAttribute(Qt::WA_TranslucentBackground, true);
            titleText->setStyleSheet(COMMON_DEFAULT_COLOR);
            titleText->setVisible(true);

            nameLineEdit->setGeometry(s(790), s(390), s(430), s(62));
            nameLineEdit->setPlaceholderText(_tr("Username"));
            nameLineEdit->setStyleSheet(COMMON_USER_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
            nameLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1)));
            nameLineEdit->setTextMargins(s(5), 0, s(24), 0);
            nameLineEdit->setVisible(true);

            pass1LineEdit->setGeometry(s(790), s(480), s(430), s(62));
            pass1LineEdit->setPlaceholderText(_tr("Password"));
            pass1LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
            pass1LineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1)));
            pass1LineEdit->setTextMargins(s(5), 0, s(24), 0);
            pass1LineEdit->setEchoMode(QLineEdit::Password);
            pass1LineEdit->setVisible(true);

            pass2LineEdit->setGeometry(s(790), s(570), s(430), s(62));
            pass2LineEdit->setPlaceholderText(_tr("Re-type password"));
            pass2LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
            pass2LineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1)));
            pass2LineEdit->setTextMargins(s(5), 0, s(24), 0);
            pass2LineEdit->setEchoMode(QLineEdit::Password);
            pass2LineEdit->setVisible(true);

            createNewUser->setGeometry(s(790), s(660), s(430), s(62));
            createNewUser->setText(_tr("CREATE NEW MANAGER USER"));
            createNewUser->setStyleSheet(COMMON_GREEN_BUTTON);
            createNewUser->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1)));
            createNewUser->setFlat(true);
            createNewUser->setCursor(Qt::PointingHandCursor);
            createNewUser->setVisible(true);

            back->setGeometry(s(790), s(750), s(430), s(62));
            back->setText(_tr("BACK"));
            back->setStyleSheet(COMMON_RED_BUTTON);
            back->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1)));
            back->setFlat(true);
            back->setCursor(Qt::PointingHandCursor);
            back->setVisible(true);

            info->setGeometry(s(578), s(906), s(28), s(28));
            info->setFlat(true);
            info->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_ButtonExclamation_Light.png); border-radius: 7px");
            info->setCursor(Qt::PointingHandCursor);
            info->setVisible(true);

            infoMessage->setGeometry(s(100), s(750), s(428), s(140));
            infoMessage->setText(_tr("CREATE NEW MANAGER USER INFO"));
            infoMessage->setStyleSheet("color : white; ");
            infoMessage->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1)));
            infoMessage->setAttribute(Qt::WA_TranslucentBackground, true);
            infoMessage->setWordWrap(true);
            infoMessage->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        } else if(currentState == STATE_CREATE_WALLET) {
            titleText->setGeometry(s(391), s(137), s(214), s(20));
            titleText->setText(_tr("CREATE NEW WALLET"));
            titleText->setStyleSheet(COMMON_DEFAULT_COLOR);
            titleText->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            titleText->setAlignment(Qt::AlignCenter);
            titleText->setAttribute(Qt::WA_TranslucentBackground, true);
            titleText->setVisible(true);

            nameLineEdit->setGeometry(s(391), s(181), s(214), s(31));
            nameLineEdit->setPlaceholderText(_tr("Wallet name"));
            nameLineEdit->setStyleSheet(COMMON_USER_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
            nameLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            nameLineEdit->setTextMargins(s(5), 0, s(24), 0);
            nameLineEdit->setVisible(true);

            pass1LineEdit->setGeometry(s(391), s(221), s(214), s(31));
            pass1LineEdit->setPlaceholderText(_tr("Password"));
            pass1LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
            pass1LineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            pass1LineEdit->setTextMargins(s(5), 0, s(24), 0);
            pass1LineEdit->setEchoMode(QLineEdit::Password);
            pass1LineEdit->setVisible(true);

            pass2LineEdit->setGeometry(s(391), s(259), s(214), s(31));
            pass2LineEdit->setPlaceholderText(_tr("Re-type password"));
            pass2LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
            pass2LineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            pass2LineEdit->setTextMargins(s(5), 0, s(24), 0);
            pass2LineEdit->setEchoMode(QLineEdit::Password);
            pass2LineEdit->setVisible(true);

            /*createNewWallet.setGeometry(s(391), s(311), s(214), s(31));
            createNewWallet.setText(_tr("CREATE NEW WALLET"));
            createNewWallet.setStyleSheet(COMMON_GREEN_BUTTON);
            createNewWallet.setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            createNewWallet.setFlat(true);
            createNewWallet.setCursor(Qt::PointingHandCursor);
            createNewWallet.setVisible(true);*/

            back->setGeometry(s(391), s(353), s(214), s(31));
            back->setText(_tr("BACK"));
            back->setStyleSheet(COMMON_RED_BUTTON);
            back->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            back->setFlat(true);
            back->setCursor(Qt::PointingHandCursor);
            back->setVisible(true);

            info->setGeometry(s(289), s(453), s(14), s(14));
            info->setFlat(true);
            info->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_ButtonExclamation_Light.png); border-radius: 7px");
            info->setCursor(Qt::PointingHandCursor);
            info->setVisible(true);

            infoMessage->setGeometry(s(50), s(375), s(214), s(70));
            infoMessage->setText(_tr("CREATE NEW WALLET INFO"));
            infoMessage->setStyleSheet("color : white; ");
            infoMessage->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            infoMessage->setAttribute(Qt::WA_TranslucentBackground, true);
            infoMessage->setWordWrap(true);
            infoMessage->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        } else if(currentState == STATE_OPEN_WALLET) {
            titleText->setText(_tr("OPEN WALLET"));
            titleText->setGeometry(s(391), s(137), s(214), s(20));
            titleText->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            titleText->setAlignment(Qt::AlignCenter);
            titleText->setAttribute(Qt::WA_TranslucentBackground, true);
            titleText->setVisible(true);

            nameLineEdit->setGeometry(s(391), s(178), s(214), s(31));
            nameLineEdit->setPlaceholderText(_tr("Wallet name"));
            nameLineEdit->setStyleSheet(COMMON_USER_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
            nameLineEdit->setTextMargins(s(5), 0, s(24), 0);
            nameLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            nameLineEdit->setVisible(true);

            pass1LineEdit->setGeometry(s(391), s(219), s(214), s(31));
            pass1LineEdit->setPlaceholderText(_tr("Password"));
            pass1LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
            pass1LineEdit->setTextMargins(s(5), 0, s(24), 0);
            pass1LineEdit->setEchoMode(QLineEdit::Password);
            pass1LineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            pass1LineEdit->setVisible(true);

            /*openWallet.setGeometry(s(391), s(311), s(214), s(31));
            openWallet.setText(_tr("OPEN WALLET"));
            openWallet.setStyleSheet(COMMON_GREEN_BUTTON);
            openWallet.setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            openWallet.setFlat(true);
            openWallet.setCursor(Qt::PointingHandCursor);
            openWallet.setVisible(true);*/

            back->setGeometry(s(391), s(353), s(214), s(31));
            back->setStyleSheet("color: white; ");
            back->setStyleSheet(COMMON_RED_BUTTON);
            back->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            back->setText(_tr("BACK"));
            back->setFlat(true);
            back->setCursor(Qt::PointingHandCursor);
            back->setVisible(true);

            info->setGeometry(s(289), s(453), s(14), s(14));
            info->setFlat(true);
            info->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_ButtonExclamation_Light.png); border-radius: 7px");
            info->setCursor(Qt::PointingHandCursor);
            info->setVisible(true);

            infoMessage->setGeometry(s(50), s(375), s(214), s(70));
            infoMessage->setText(_tr("OPEN WALLET INFO"));
            infoMessage->setStyleSheet("color : white; ");
            infoMessage->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            infoMessage->setAttribute(Qt::WA_TranslucentBackground, true);
            infoMessage->setWordWrap(true);
            infoMessage->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        } else if(currentState == STATE_RESTORE_WALLET) {
            titleText->setGeometry(s(391), s(137), s(214), s(20));
            titleText->setText(_tr("RESTORE WALLET"));
            titleText->setStyleSheet(COMMON_DEFAULT_COLOR);
            titleText->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            titleText->setAlignment(Qt::AlignCenter);
            titleText->setAttribute(Qt::WA_TranslucentBackground, true);
            titleText->setVisible(true);

            privKeyLineEdit->setGeometry(s(391), s(181), s(214), s(31));
            privKeyLineEdit->setPlaceholderText(_tr("Private KEY"));
            privKeyLineEdit->setStyleSheet(COMMON_PRIVATE_KEY_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
            privKeyLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            privKeyLineEdit->setTextMargins(s(5), 0, s(24), 0);
            privKeyLineEdit->setVisible(true);

            nameLineEdit->setGeometry(s(391), s(221), s(214), s(31));
            nameLineEdit->setPlaceholderText(_tr("Wallet name"));
            nameLineEdit->setStyleSheet(COMMON_USER_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
            nameLineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            nameLineEdit->setTextMargins(s(5), 0, s(24), 0);
            nameLineEdit->setVisible(true);

            pass1LineEdit->setGeometry(s(391), s(261), s(214), s(31));
            pass1LineEdit->setPlaceholderText(_tr("Password"));
            pass1LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
            pass1LineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            pass1LineEdit->setTextMargins(s(5), 0, s(24), 0);
            pass1LineEdit->setEchoMode(QLineEdit::Password);
            pass1LineEdit->setVisible(true);

            pass2LineEdit->setGeometry(s(391), s(301), s(214), s(31));
            pass2LineEdit->setPlaceholderText(_tr("Re-type password"));
            pass2LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
            pass2LineEdit->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            pass2LineEdit->setTextMargins(s(5), 0, s(24), 0);
            pass2LineEdit->setEchoMode(QLineEdit::Password);
            pass2LineEdit->setVisible(true);

            /*restoreWallet.setGeometry(s(391), s(341), s(214), s(31));
            restoreWallet.setText(_tr("RESTORE WALLET"));
            restoreWallet.setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight()));
            restoreWallet.setStyleSheet(COMMON_GREEN_BUTTON);
            restoreWallet.setFlat(true);
            restoreWallet.setCursor(Qt::PointingHandCursor);
            restoreWallet.setVisible(true);*/

            back->setGeometry(s(391), s(381), s(214), s(31));
            back->setText(_tr("BACK"));
            back->setStyleSheet(COMMON_RED_BUTTON);
            back->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            back->setFlat(true);
            back->setCursor(Qt::PointingHandCursor);
            back->setVisible(true);

            info->setGeometry(s(289), s(453), s(14), s(14));
            info->setFlat(true);
            info->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_ButtonExclamation_Light.png); border-radius: 7px");
            info->setCursor(Qt::PointingHandCursor);
            info->setVisible(true);

            infoMessage->setGeometry(s(50), s(375), s(214), s(70));
            infoMessage->setText(_tr("RESTORE WALLET INFO"));
            infoMessage->setStyleSheet("color : white; ");
            infoMessage->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
            infoMessage->setAttribute(Qt::WA_TranslucentBackground, true);
            infoMessage->setWordWrap(true);
            infoMessage->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        }
        widget->setGeometry(0, 0, mdiArea->width(), mdiArea->height());
        window->setGeometry(0, 0, mdiArea->width(), mdiArea->height());

        mdiArea->repaint();
        pastState = currentState;
    }
}

void login::on_privateKeyLineEdit_textChanged(const QString &arg1) {
    privKey = arg1;
    if (!check::privateKey(arg1)) {
        privKeyLineEdit->setStyleSheet(COMMON_PRIVATE_KEY_LINE_EDIT_PICTURE COMMON_STYLE COMMON_ERROR_COLOR);
    } else {
        privKeyLineEdit->setStyleSheet(COMMON_PRIVATE_KEY_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
    }
}

void login::on_nameNameLineEdit_textChanged(const QString &arg1) {
    name = arg1;
    if (!check::name(arg1)) {
        nameLineEdit->setStyleSheet(COMMON_USER_LINE_EDIT_PICTURE COMMON_STYLE COMMON_ERROR_COLOR);
    } else {
        nameLineEdit->setStyleSheet(COMMON_USER_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
    }
}

void login::on_pass1LineEdit_textChanged(const QString &arg1) {
    pass1 = arg1;
    if(!check::password(arg1)) {
        pass1LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_ERROR_COLOR);
    } else {
        pass1LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
    }
    pass2LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
    if(!check::password(pass1, pass2)) {
        pass2LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_ERROR_COLOR);
    } else {
        pass2LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
    }
}

void login::on_pass2LineEdit_textChanged(const QString &arg1) {
    pass2 = arg1;
    if(!check::password(pass1, pass2)) {
        pass2LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_ERROR_COLOR);
    } else {
        pass2LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_DEFAULT_COLOR);
    }
}

void login::on_createNewWallet_pushed() {
    if(currentState == STATE_LOGIN_PAGE)
        currentState = STATE_CREATE_WALLET;
    else {
        if (!check::name(name)) {
            nameLineEdit->setStyleSheet(COMMON_USER_LINE_EDIT_PICTURE COMMON_STYLE COMMON_ERROR_COLOR);
        } else if(!check::password(pass1, pass2)) {
            pass1LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_ERROR_COLOR);
            pass2LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_ERROR_COLOR);
        } else {
            events::setWalletUserPassword(QPair<QString, QString>({name, pass1}));
            if(!walletfile::exists(events::getWalletUserPassword().first)) {
                events::setWalletHistoryChanged();
                if(walletfile::save(events::getWalletUserPassword().first, events::getWalletUserPassword().second)) {
                    currentState = STATE_LOGIN_PAGE;
                    //command = command_e::CMD_CREATE_WALLET;
                }
            }
        }
    }
}

void login::on_openWallet_pushed() {
    if(currentState == STATE_LOGIN_PAGE)
        currentState = STATE_OPEN_WALLET;
    else {
        if (!check::name(name)) {
            nameLineEdit->setStyleSheet(COMMON_USER_LINE_EDIT_PICTURE COMMON_STYLE COMMON_ERROR_COLOR);
        } else {
            currentState = STATE_NONE;
            command = command_e::CMD_OPEN_WALLET;
        }
    }
}

void login::on_restoreWallet_pushed() {
    if(currentState == STATE_LOGIN_PAGE)
        currentState = STATE_RESTORE_WALLET;
    else {
        if (!check::name(name)) {
            nameLineEdit->setStyleSheet(COMMON_USER_LINE_EDIT_PICTURE COMMON_STYLE COMMON_ERROR_COLOR);
        } else if(!check::password(pass1, pass2)) {
            pass1LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_ERROR_COLOR);
            pass2LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_ERROR_COLOR);
        } else {
            if (!check::privateKey(privKey)) {
                privKeyLineEdit->setStyleSheet(COMMON_PRIVATE_KEY_LINE_EDIT_PICTURE COMMON_STYLE COMMON_ERROR_COLOR);
            } else {
                currentState = STATE_NONE;
                command = command_e::CMD_RESTORE_WALLET;
            }
        }
    }
}

void login::on_login_pushed() {
    if(actionSemaphore) {
        return;
    }
    actionSemaphore = true;
    if(currentState == STATE_LOGIN_PAGE) {
        if (!check::name(name)) {
            nameLineEdit->setStyleSheet(COMMON_USER_LINE_EDIT_PICTURE COMMON_STYLE COMMON_ERROR_COLOR);
        } else {
            if(!check::password(pass1)) {
                pass1LineEdit->setStyleSheet(COMMON_PASS_LINE_EDIT_PICTURE COMMON_STYLE COMMON_ERROR_COLOR);
            } else {
                events::setWalletUserPassword(QPair<QString, QString>({name, pass1}));
                loginManager->setText(_tr("WAIT"));
                loginManager->repaint();
                if(walletfile::load(events::getWalletUserPassword().first, events::getWalletUserPassword().second)) {
                    currentState = STATE_NONE;
                    command = command_e::CMD_LOGIN_WALLET;

                    wallethistory::updateAccounts();
                    walletfile::save(events::getWalletUserPassword().first, events::getWalletUserPassword().second);
                    events::setUpdateHistory();
                    loginManager->setText(_tr("LOG IN"));
                } else {
                    loginManager->setStyleSheet(COMMON_RED_BUTTON);
                    loginManager->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1)));
                    loginManager->setText(_tr("ERROR!"));
                    loginManager->repaint();
                    QThread::sleep(2);
                    loginManager->setStyleSheet(COMMON_GREEN_BUTTON);
                    loginManager->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1)));
                    loginManager->setText(_tr("LOG IN"));

                }
            }
        }
    }
    actionSemaphore = false;;
}

void login::on_pass1EnterPushed() {
    on_login_pushed();
}

void login::on_getStarted_pushed() {
    if(currentState == STATE_LOGIN_PAGE)
        currentState = STATE_CREATE_MANAGER;
}

void login::on_back_pushed() {
    currentState = STATE_LOGIN_PAGE;
}

void login::on_info_pushed() {
    if(infoSwitch) {
        infoSwitch = false;
        window->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_Light.png)");
        infoMessage->setText("");
        infoMessage->setVisible(false);
    } else {
        infoSwitch = true;
        window->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/loginCommon/main_Info_Light.png)");
        if(currentState == STATE_LOGIN_PAGE) {
            infoMessage->setText(_tr("LOG IN INFO"));
        } else if(currentState == STATE_CREATE_MANAGER) {
            infoMessage->setText(_tr("CREATE NEW MANAGER USER INFO"));
        } else if(currentState == STATE_CREATE_WALLET) {
            infoMessage->setText(_tr("CREATE NEW WALLET INFO"));
        } else if(currentState == STATE_OPEN_WALLET) {
            infoMessage->setText(_tr("OPEN WALLET INFO"));
        } else if(currentState == STATE_RESTORE_WALLET) {
            infoMessage->setText(_tr("RESTORE WALLET INFO"));
        }
        infoMessage->setVisible(true);
    }
}

