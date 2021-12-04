#include "mainboard.h"
#include <QMdiSubWindow>
#include <QApplication>
#include <QEvent>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QMouseEvent>
#include <QLineEdit>

#include "language/translate.h"
#include "wallet/events.h"
#include "crypto/signatures.h"
#include "wallet/rpc/wallethistory.h"
#include "pages/pc/textformating.h"
#include "pages/pc/populate.h"

#define COMMON_SUBWINDOW_LEFT_SPACE 100
#define COMMON_SUBWINDOW_UP_SPACE 0
#define COMMON_LOCATION_AND_SIZE        s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), mdiAreaMain.width() - s(LEFT_MENU_WIDTH), mdiAreaMain.height() - s(HEADER_HEIGHT)

#define s(s) _scale(s)

mainboard::mainboard() {

}

mainboard::~mainboard() {

}

void mainboard::setVars(QMdiArea *mdiArea, QWidget *parent) {
    this->parent = parent;
    this->mdiArea = mdiArea;

    windowMain = new QMdiSubWindow(mdiArea);
    mdiAreaHeader = new QMdiArea(windowMain);
    widgetHeader = new QWidget(windowMain);
    widgetHeader->show();
    windowHeader = new QMdiSubWindow(windowMain);
    windowHeader->setWidget(widgetHeader);

    mdiAreaLeftMenu = new QMdiArea(windowMain);
    widgetLeftMenu = new QWidget(windowMain);
    widgetLeftMenu->show();
    windowLeftMenu = new QMdiSubWindow(windowMain);
    windowLeftMenu->setWidget(widgetLeftMenu);

    userNameLabel = new QLabel(windowMain);
/****** Wallet selector ******************************************/
    networkLabelImage = new QLabel(windowMain);
    networkLabel = new QLabel(windowMain);
    walletSelectorLabel = new QLabel(windowMain);
    walletSelectorComboBox = new QComboBox(windowMain);
    connect(walletSelectorComboBox, SIGNAL(currentTextChanged(const QString &)),this, SLOT(on_Account_Changed(const QString &)));
    updateWallet();
    updateNetwork();

    bellImageLabel = new QPushButton(windowMain);
    bellNrLabel = new QLabel(windowMain);
    messageImageLabel = new QLabel(windowMain);
    messageNrLabel = new QLabel(windowMain);
    updateBell();
    updateMessages();
    //events::setBells(25);
    //events::setMessages(90);

    userLabel = new QLabel(windowMain);
    updateUser();

    logOutButton = new QPushButton(windowMain);
    connect(logOutButton, SIGNAL(clicked()),this, SLOT(on_logOut_ButtonPressed()));
    updateLogOut();

    connect(bellImageLabel, SIGNAL(clicked()),this, SLOT(on_ring_pushed()));

    userNameLabel->setStyleSheet("color: #eee;");
    userNameLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    userNameLabel->setGeometry(s(1168), s(22), s(150), s(25));
    userNameLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.0)));
    userNameLabel->setText(events::getWalletUserPassword().first);

    dashboardIcoLabel = new QLabel(windowLeftMenu);
    dashboardTextLabel = new QLabel(windowLeftMenu);
    dashboardArowLabel = new QLabel(windowLeftMenu);
    dashboardButton = new QLabel(windowLeftMenu);

    myWalletIcoLabel = new QLabel(windowLeftMenu);
    myWalletTextLabel = new QLabel(windowLeftMenu);
    myWalletArowLabel = new QLabel(windowLeftMenu);
    myWalletButton = new QLabel(windowLeftMenu);

    myWalletReceiveIcoLabel = new QLabel(windowLeftMenu);
    myWalletReceiveTextLabel = new QLabel(windowLeftMenu);
    myWalletReceiveButton = new QLabel(windowLeftMenu);

    myWalletSendIcoLabel = new QLabel(windowLeftMenu);
    myWalletSendTextLabel = new QLabel(windowLeftMenu);
    myWalletSendButton = new QLabel(windowLeftMenu);

    lyraTokensIcoLabel = new QLabel(windowLeftMenu);
    lyraTokensTextLabel = new QLabel(windowLeftMenu);
    lyraTokensArowLabel = new QLabel(windowLeftMenu);
    lyraTokenButton = new QLabel(windowLeftMenu);

    swapIcoLabel = new QLabel(windowLeftMenu);
    swapTextLabel = new QLabel(windowLeftMenu);
    swapArowLabel = new QLabel(windowLeftMenu);
    swapButton = new QLabel(windowLeftMenu);

    transactionsIcoLabel = new QLabel(windowLeftMenu);
    transactionsTextLabel = new QLabel(windowLeftMenu);
    transactionsArowLabel = new QLabel(windowLeftMenu);
    transactionsButton = new QLabel(windowLeftMenu);

    searchIcoLabel = new QLabel(windowLeftMenu);
    searchTextLabel = new QLabel(windowLeftMenu);
    searchArowLabel = new QLabel(windowLeftMenu);
    searchButton = new QLabel(windowLeftMenu);

    settingsIcoLabel = new QLabel(windowLeftMenu);
    settingsTextLabel = new QLabel(windowLeftMenu);
    settingsArowLabel = new QLabel(windowLeftMenu);
    settingsButton = new QLabel(windowLeftMenu);

    infoButton = new QPushButton(windowLeftMenu);
    infoLabel = new QLabel(windowLeftMenu);

    dashboardIcoLabel->setScaledContents(true);
    dashboardIcoLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    dashboardIcoLabel->setCursor(Qt::PointingHandCursor);
    dashboardIcoLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    dashboardTextLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    dashboardTextLabel->setCursor(Qt::PointingHandCursor);
    dashboardTextLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    dashboardArowLabel->setScaledContents(true);
    dashboardArowLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    dashboardArowLabel->setCursor(Qt::PointingHandCursor);
    dashboardArowLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    dashboardButton->setCursor(Qt::PointingHandCursor);
    dashboardButton->setAttribute(Qt::WA_TranslucentBackground, true);

    myWalletIcoLabel->setScaledContents(true);
    myWalletIcoLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    myWalletIcoLabel->setCursor(Qt::PointingHandCursor);
    myWalletIcoLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    myWalletTextLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    myWalletTextLabel->setCursor(Qt::PointingHandCursor);
    myWalletTextLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    myWalletArowLabel->setScaledContents(true);
    myWalletArowLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    myWalletArowLabel->setCursor(Qt::PointingHandCursor);
    myWalletArowLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    myWalletButton->setCursor(Qt::PointingHandCursor);
    myWalletButton->setAttribute(Qt::WA_TranslucentBackground, true);


    myWalletReceiveIcoLabel->setScaledContents(true);
    myWalletReceiveIcoLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    myWalletReceiveIcoLabel->setCursor(Qt::PointingHandCursor);
    myWalletReceiveIcoLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    myWalletReceiveTextLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    myWalletReceiveTextLabel->setCursor(Qt::PointingHandCursor);
    myWalletReceiveTextLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    myWalletReceiveButton->setCursor(Qt::PointingHandCursor);
    myWalletReceiveButton->setAttribute(Qt::WA_TranslucentBackground, true);


    myWalletSendIcoLabel->setScaledContents(true);
    myWalletSendIcoLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    myWalletSendIcoLabel->setCursor(Qt::PointingHandCursor);
    myWalletSendIcoLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    myWalletSendTextLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    myWalletSendTextLabel->setCursor(Qt::PointingHandCursor);
    myWalletSendTextLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    myWalletSendButton->setCursor(Qt::PointingHandCursor);
    myWalletSendButton->setAttribute(Qt::WA_TranslucentBackground, true);


    lyraTokensIcoLabel->setScaledContents(true);
    lyraTokensIcoLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    lyraTokensIcoLabel->setCursor(Qt::PointingHandCursor);
    lyraTokensIcoLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    lyraTokensTextLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    lyraTokensTextLabel->setCursor(Qt::PointingHandCursor);
    lyraTokensTextLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    lyraTokensArowLabel->setScaledContents(true);
    lyraTokensArowLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    lyraTokensArowLabel->setCursor(Qt::PointingHandCursor);
    lyraTokensArowLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    lyraTokenButton->setCursor(Qt::PointingHandCursor);
    lyraTokenButton->setAttribute(Qt::WA_TranslucentBackground, true);
#if LYRA_TOKEN_PAGE_IMPLEMENTED == 0
    lyraTokensIcoLabel->setVisible(false);
    lyraTokensTextLabel->setVisible(false);
    lyraTokensArowLabel->setVisible(false);
    lyraTokenButton->setVisible(false);
#endif
    swapIcoLabel->setScaledContents(true);
    swapIcoLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    swapIcoLabel->setCursor(Qt::PointingHandCursor);
    swapIcoLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    swapTextLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    swapTextLabel->setCursor(Qt::PointingHandCursor);
    swapTextLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    swapArowLabel->setScaledContents(true);
    swapArowLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    swapArowLabel->setCursor(Qt::PointingHandCursor);
    swapArowLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    swapButton->setCursor(Qt::PointingHandCursor);
    swapButton->setAttribute(Qt::WA_TranslucentBackground, true);
#if SWAP_PAGE_IMPLEMENTED == 0
    swapIcoLabel->setVisible(false);
    swapTextLabel->setVisible(false);
    swapArowLabel->setVisible(false);
    swapButton->setVisible(false);
#endif
    transactionsIcoLabel->setScaledContents(true);
    transactionsIcoLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    transactionsIcoLabel->setCursor(Qt::PointingHandCursor);
    transactionsIcoLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    transactionsTextLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    transactionsTextLabel->setCursor(Qt::PointingHandCursor);
    transactionsTextLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    transactionsArowLabel->setScaledContents(true);
    transactionsArowLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    transactionsArowLabel->setCursor(Qt::PointingHandCursor);
    transactionsArowLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    transactionsButton->setCursor(Qt::PointingHandCursor);
    transactionsButton->setAttribute(Qt::WA_TranslucentBackground, true);
#if TRANSITIONS_PAGE_IMPLEMENTED == 0
    transactionsIcoLabel->setVisible(false);
    transactionsTextLabel->setVisible(false);
    transactionsArowLabel->setVisible(false);
    transactionsButton->setVisible(false);
#endif
    searchIcoLabel->setScaledContents(true);
    searchIcoLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    searchIcoLabel->setCursor(Qt::PointingHandCursor);
    searchIcoLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    searchTextLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    searchTextLabel->setCursor(Qt::PointingHandCursor);
    searchTextLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    searchArowLabel->setScaledContents(true);
    searchArowLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    searchArowLabel->setCursor(Qt::PointingHandCursor);
    searchArowLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    searchButton->setCursor(Qt::PointingHandCursor);
    searchButton->setAttribute(Qt::WA_TranslucentBackground, true);
#if SEARCH_PAGE_IMPLEMENTED == 0
    searchIcoLabel->setVisible(false);
    searchTextLabel->setVisible(false);
    searchArowLabel->setVisible(false);
    searchButton->setVisible(false);
#endif
    settingsIcoLabel->setStyleSheet("color: #aaa;");
    settingsIcoLabel->setScaledContents(true);
    settingsIcoLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    settingsIcoLabel->setCursor(Qt::PointingHandCursor);
    settingsIcoLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    settingsTextLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    settingsTextLabel->setCursor(Qt::PointingHandCursor);
    settingsTextLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    settingsArowLabel->setStyleSheet("color: #aaa;");
    settingsArowLabel->setScaledContents(true);
    settingsArowLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    settingsArowLabel->setCursor(Qt::PointingHandCursor);
    settingsArowLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    settingsButton->setCursor(Qt::PointingHandCursor);
    settingsButton->setAttribute(Qt::WA_TranslucentBackground, true);

    infoButton->setCursor(Qt::PointingHandCursor);
    infoButton->setAttribute(Qt::WA_TranslucentBackground, true);
    connect(infoButton, SIGNAL(clicked()),this, SLOT(on_info_ButtonPressed()));

    infoLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    infoLabel->setCursor(Qt::PointingHandCursor);
    infoLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    infoLabel->setStyleSheet("background-color: transparent; color: #aaa; ");
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setText("lyra.inc");


/***** !Wallet selector *****************************************/
    updateObjects();
    updateMenuButtonsSize();
    updateMenuButtonsLanguage();
    updateMenuButtonsSelected();

    //dashboardWindow = new dashboardpage();
    //dashboardWindow->setVars(windowMain, parent);
    //walletWindow = new walletpage();
    //walletWindow->setVars(windowMain, parent);
    //settingsWindow = new settingswindow();
    //settingsWindow->setVars(windowMain, parent);
    //transitionsWindow = new transitionswindow();
    //transitionsWindow->setVars(windowMain, parent);
    windowMain->setVisible(false);
}

mainboard::command_e mainboard::getCommand() {
    command_e cmd = command;
    command = command_e::CMD_NONE;
    return cmd;
}

void mainboard::setState(state_e state) {
    currentState = state;
}

void mainboard::updateMenuButtonsSelected() {
    QPixmap green(":/resource/ico/" + events::getStyle() + "/mainDashBoard/arow_green.png");
    QPixmap grey(":/resource/ico/" + events::getStyle() + "/mainDashBoard/arow_grey.png");

    if(currentState == state_e::STATE_DASHBOARD) {
        QPixmap image1(":/resource/ico/" + events::getStyle() + "/mainDashBoard/icoDashboard_green.png");
        dashboardIcoLabel->setPixmap(image1);
        dashboardTextLabel->setStyleSheet("color: #40B630;");
        dashboardArowLabel->setPixmap(green);
    } else {
        QPixmap image1(":/resource/ico/" + events::getStyle() + "/mainDashBoard/icoDashboard_grey.png");
        dashboardIcoLabel->setPixmap(image1);
        dashboardTextLabel->setStyleSheet("color: #999;");
        dashboardArowLabel->setPixmap(grey);
    }

    if(currentState == state_e::STATE_MY_WALLET) {
        myWalletTextLabel->setStyleSheet("color: #40B630;");
    } else {
        myWalletTextLabel->setStyleSheet("color: #999;");
    }
    if(currentState == state_e::STATE_MY_WALLET || currentState == state_e::STATE_MY_WALLET_RECEIVE || currentState == state_e::STATE_MY_WALLET_SEND) {
        QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/icoMyWallet_green.png");
        myWalletIcoLabel->setPixmap(image2);
        myWalletArowLabel->setPixmap(green);
    } else {
        QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/icoMyWallet_grey.png");
        myWalletIcoLabel->setPixmap(image2);
        myWalletArowLabel->setPixmap(grey);
    }
    if(currentState == state_e::STATE_MY_WALLET_RECEIVE) {
        QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/receive.png");
        myWalletReceiveIcoLabel->setPixmap(image2);
        myWalletReceiveTextLabel->setStyleSheet("color: #40B630;");
    } else {
        QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/receive.png");
        myWalletReceiveIcoLabel->setPixmap(image2);
        myWalletReceiveTextLabel->setStyleSheet("color: #999;");
    }
    if(currentState == state_e::STATE_MY_WALLET_SEND) {
        QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/send.png");
        myWalletSendIcoLabel->setPixmap(image2);
        myWalletSendTextLabel->setStyleSheet("color: #40B630;");
    } else {
        QPixmap image2(":/resource/ico/" + events::getStyle() + "/mainDashBoard/send.png");
        myWalletSendIcoLabel->setPixmap(image2);
        myWalletSendTextLabel->setStyleSheet("color: #999;");
    }

    if(currentState == state_e::STATE_LYRA_TOKENS) {
        QPixmap image3(":/resource/ico/" + events::getStyle() + "/mainDashBoard/icoLyraTokens_green.png");
        lyraTokensIcoLabel->setPixmap(image3);
        lyraTokensTextLabel->setStyleSheet("color: #40B630;");
        lyraTokensArowLabel->setPixmap(green);
    } else {
        QPixmap image3(":/resource/ico/" + events::getStyle() + "/mainDashBoard/icoLyraTokens_grey.png");
        lyraTokensIcoLabel->setPixmap(image3);
        lyraTokensTextLabel->setStyleSheet("color: #999;");
        lyraTokensArowLabel->setPixmap(grey);
    }

    if(currentState == state_e::STATE_SWAP) {
        QPixmap image4(":/resource/ico/" + events::getStyle() + "/mainDashBoard/icoSwap_green.png");
        swapIcoLabel->setPixmap(image4);
        swapTextLabel->setStyleSheet("color: #40B630;");
        swapArowLabel->setPixmap(green);
    } else {
        QPixmap image4(":/resource/ico/" + events::getStyle() + "/mainDashBoard/icoSwap_grey.png");
        swapIcoLabel->setPixmap(image4);
        swapTextLabel->setStyleSheet("color: #999;");
        swapArowLabel->setPixmap(grey);
    }

    if(currentState == state_e::STATE_TRANSITION) {
        QPixmap image5(":/resource/ico/" + events::getStyle() + "/mainDashBoard/icoTransition_green.png");
        transactionsIcoLabel->setPixmap(image5);
        transactionsTextLabel->setStyleSheet("color: #40B630;");
        transactionsArowLabel->setPixmap(green);
    } else {
        QPixmap image5(":/resource/ico/" + events::getStyle() + "/mainDashBoard/icoTransition_grey.png");
        transactionsIcoLabel->setPixmap(image5);
        transactionsTextLabel->setStyleSheet("color: #999;");
        transactionsArowLabel->setPixmap(grey);
    }

    if(currentState == state_e::STATE_SEARCH) {
        QPixmap image6(":/resource/ico/" + events::getStyle() + "/mainDashBoard/icoSearch_green.png");
        searchIcoLabel->setPixmap(image6);
        searchTextLabel->setStyleSheet("color: #40B630;");
        searchArowLabel->setPixmap(green);
    } else {
        QPixmap image6(":/resource/ico/" + events::getStyle() + "/mainDashBoard/icoSearch_grey.png");
        searchIcoLabel->setPixmap(image6);
        searchTextLabel->setStyleSheet("color: #999;");
        searchArowLabel->setPixmap(grey);
    }

    if(currentState == state_e::STATE_SETTINGS) {
        QPixmap image7(":/resource/ico/" + events::getStyle() + "/mainDashBoard/icoSettings_green.png");
        settingsIcoLabel->setPixmap(image7);
        settingsTextLabel->setStyleSheet("color: #40B630;");
        settingsArowLabel->setPixmap(green);
    } else {
        QPixmap image7(":/resource/ico/" + events::getStyle() + "/mainDashBoard/icoSettings_grey.png");
        settingsIcoLabel->setPixmap(image7);
        settingsTextLabel->setStyleSheet("color: #999;");
        settingsArowLabel->setPixmap(grey);
    }
}

void mainboard::updateMenuButtonsFont() {
    dashboardTextLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.0)));
    myWalletTextLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.0)));
    myWalletReceiveTextLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    myWalletSendTextLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    lyraTokensTextLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.0)));
    swapTextLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.0)));
    transactionsTextLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.0)));
    searchTextLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.0)));
    settingsTextLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.0)));

}
void mainboard::updateMenuButtonsSize() {
    int yCount = 0;
    dashboardIcoLabel->setGeometry(s(28), s(yCount + 30), s(30), s(30));
    dashboardTextLabel->setGeometry(s(72), s(yCount + 28), s(150), s(30));
    if(currentState == state_e::STATE_DASHBOARD)
        dashboardArowLabel->setGeometry(s(205), s(yCount + 39), s(10), s(6));
    else
        dashboardArowLabel->setGeometry(s(205), s(yCount + 41), s(6), s(10));
    dashboardButton->setGeometry(s(0), s(yCount + 20), s(210), s(53));

    yCount += 73;

    myWalletIcoLabel->setGeometry(s(28), s(yCount + 30), s(30), s(30));
    myWalletTextLabel->setGeometry(s(72), s(yCount + 30), s(150), s(30));
    if(currentState == state_e::STATE_MY_WALLET || currentState == state_e::STATE_MY_WALLET_RECEIVE || currentState == state_e::STATE_MY_WALLET_SEND)
        myWalletArowLabel->setGeometry(s(205), s(yCount + 39), s(10), s(6));
    else
        myWalletArowLabel->setGeometry(s(205), s(yCount + 41), s(6), s(10));
    if(currentState == STATE_MY_WALLET || currentState == STATE_MY_WALLET_SEND || currentState == STATE_MY_WALLET_RECEIVE)
        myWalletButton->setGeometry(s(0), s(yCount + 20), s(210), s(35));
    else
        myWalletButton->setGeometry(s(0), s(yCount + 20), s(210), s(53));

    if(currentState == STATE_MY_WALLET || currentState == STATE_MY_WALLET_SEND || currentState == STATE_MY_WALLET_RECEIVE)
        yCount += 53;

    myWalletSendIcoLabel->setGeometry(s(77), s(yCount + 15), s(15), s(15));
    myWalletSendTextLabel->setGeometry(s(100), s(yCount + 8), s(150), s(30));
    myWalletSendButton->setGeometry(s(0), s(yCount + 15), s(210), s(15));

    if(currentState == STATE_MY_WALLET || currentState == STATE_MY_WALLET_SEND || currentState == STATE_MY_WALLET_RECEIVE)
        yCount += 33;

    myWalletReceiveIcoLabel->setGeometry(s(77), s(yCount + 15), s(15), s(15));
    myWalletReceiveTextLabel->setGeometry(s(100), s(yCount + 8), s(150), s(30));
    myWalletReceiveButton->setGeometry(s(0), s(yCount + 15), s(210), s(15));

    if(currentState == STATE_MY_WALLET || currentState == STATE_MY_WALLET_SEND || currentState == STATE_MY_WALLET_RECEIVE)
        yCount += 53;
    else
        yCount += 73;
#if LYRA_TOKEN_PAGE_IMPLEMENTED
    lyraTokensIcoLabel->setGeometry(s(28), s(yCount + 30), s(30), s(30));
    lyraTokensTextLabel->setGeometry(s(72), s(yCount + 28), s(150), s(30));
    if(currentState == state_e::STATE_LYRA_TOKENS)
        lyraTokensArowLabel->setGeometry(s(205), s(yCount + 39), s(10), s(6));
    else
        lyraTokensArowLabel->setGeometry(s(205), s(yCount + 41), s(6), s(10));
    lyraTokenButton->setGeometry(s(0), s(yCount + 20), s(210), s(53));

    yCount += 73;
#endif
#if SWAP_PAGE_IMPLEMENTED
    swapIcoLabel->setGeometry(s(28), s(yCount + 30), s(30), s(30));
    swapTextLabel->setGeometry(s(72), s(yCount + 28), s(150), s(30));
    if(currentState == state_e::STATE_SWAP)
        swapArowLabel->setGeometry(s(205), s(yCount + 39), s(10), s(6));
    else
        swapArowLabel->setGeometry(s(205), s(yCount + 41), s(6), s(10));
    swapButton->setGeometry(s(0), s(yCount + 20), s(210), s(53));

    yCount += 73;
#endif
#if TRANSITIONS_PAGE_IMPLEMENTED
    transactionsIcoLabel->setGeometry(s(28), s(yCount + 30), s(30), s(30));
    transactionsTextLabel->setGeometry(s(72), s(yCount + 28), s(150), s(30));
    if(currentState == state_e::STATE_TRANSITION)
        transactionsArowLabel->setGeometry(s(205), s(yCount + 39), s(10), s(6));
    else
        transactionsArowLabel->setGeometry(s(205), s(yCount + 41), s(6), s(10));
    transactionsButton->setGeometry(s(0), s(yCount + 20), s(210), s(53));

    yCount += 73;
#endif
#if SEARCH_PAGE_IMPLEMENTED
    searchIcoLabel->setGeometry(s(28), s(yCount + 30), s(30), s(30));
    searchTextLabel->setGeometry(s(72), s(yCount + 28), s(150), s(30));
    if(currentState == state_e::STATE_SEARCH)
        searchArowLabel->setGeometry(s(205), s(yCount + 39), s(10), s(6));
    else
        searchArowLabel->setGeometry(s(205), s(yCount + 41), s(6), s(10));
    searchButton->setGeometry(s(0), s(yCount + 20), s(210), s(53));
#endif
    settingsIcoLabel->setGeometry(s(28), s(690 + 30), s(30), s(30));
    settingsTextLabel->setGeometry(s(72), s(690 + 28), s(150), s(30));
    if(currentState == state_e::STATE_SETTINGS)
        settingsArowLabel->setGeometry(s(205), s(690 + 39), s(10), s(6));
    else
        settingsArowLabel->setGeometry(s(205), s(690 + 41), s(6), s(10));
    settingsButton->setGeometry(s(0), s(690 + 20), s(210), s(53));

    infoButton->setGeometry(s(68), s(800), s(74), s(40));
    infoLabel->setGeometry(s(68), s(800), s(74), s(40));

    updateMenuButtonsFont();
}

void mainboard::updateMenuButtonsLanguage() {
    dashboardTextLabel->setText(_tr("Dashboard"));
    myWalletTextLabel->setText(_tr("My Account"));
    myWalletReceiveTextLabel->setText(_tr("Receive"));
    myWalletSendTextLabel->setText(_tr("Send"));
    lyraTokensTextLabel->setText(_tr("Lyra Tokens"));
    swapTextLabel->setText(_tr("Swap"));
    transactionsTextLabel->setText(_tr("Transactions"));
    searchTextLabel->setText(_tr("Search"));
    settingsTextLabel->setText(_tr("Settings"));

    updateMenuButtonsFont();
}

void mainboard::updateWallet() {
    walletSelectorLabel->setStyleSheet("color: #eee;");
    walletSelectorLabel->setText(_tr("Wallet") + ":");
    walletSelectorLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.1)));
    walletSelectorLabel->setAlignment(Qt::AlignRight);
    walletSelectorLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    walletSelectorLabel->setGeometry(s(276), s(43), s(170), s(37));

    walletSelectorComboBox->setGeometry(s(472), s(38), s(270), s(37));
    //walletSelectorComboBox.setCurrentIndex(0);
    walletSelectorComboBox->setAutoFillBackground(false);
    //
    walletSelectorComboBox->setStyleSheet(""
        "QComboBox {   "
               "color: #777;"
               "border-color: white;"
               "background-color: white;"
               "border: 1px solid #eee;"
               "border-radius: " + QString::number((int)s(3)) + "px;"
               "padding: 1px " + QString::number((int)s(18)) + "px 1px 3px;"
               "text-align: center;"
               ";}"
        "QComboBox::drop-down {border-width: 1px;} "
        "QComboBox::down-arrow {image: url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/walletComboBoxArrow.png);}"
        "QComboBox QAbstractItemView {"
               "border: " + QString::number((int)s(2)) + "px solid darkgray;"
               "color: #aaa;"
               "padding: 1px 1px 1px 1px;"
               "selection-background-color: darkgray;"
               "}"
    );
    walletSelectorComboBox->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(1.0)));
    /*for (int i = 0 ; i < walletSelectorComboBox.count() ; ++i) {
        walletSelectorComboBox.setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }*/
    walletSelectorComboBox->setCursor(Qt::PointingHandCursor);

}

void mainboard::updateNetwork() {
    if(events::getNetworkConnected()) {
        QPixmap image(":/resource/ico/" + events::getStyle() + "/mainDashBoard/networkConnected.png");
        networkLabelImage->setPixmap(image);
    } else {
        QPixmap image(":/resource/ico/" + events::getStyle() + "/mainDashBoard/networkDisconnected.png");
        networkLabelImage->setPixmap(image);
    }
    networkLabelImage->setAlignment(Qt::AlignCenter);
    networkLabelImage->setScaledContents(true);
    networkLabelImage->setGeometry(s(843), s(44), s(25), s(25));

    networkLabel->setStyleSheet("color: #eee;");
    /*if(events::getRpcNetwork() == events::network_e::NETWORK_DEV)
        networkLabel.setText(_tr("Dev network"));*/
    if(events::getRpcNetwork() == events::network_e::NETWORK_TESTNET)
        networkLabel->setText(_tr("Testnet network"));
    if(events::getRpcNetwork() == events::network_e::NETWORK_MAINNET)
        networkLabel->setText(_tr("Mainnet network"));

    networkLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.8)));
    networkLabel->setAlignment(Qt::AlignCenter);
    networkLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    networkLabel->setGeometry(s(853), s(36), s(170), s(37));
}

void mainboard::updateBell() {
    if(events::getBells() == 0) {
        //QPixmap image(":/resource/ico/" + events::getStyle() + "/mainDashBoard/bell.png");
        //bellImageLabel->setPixmap(image);
        bellImageLabel->setFlat(true);
        bellImageLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/bell.png); border-radius: 0px");
        bellImageLabel->setCursor(Qt::PointingHandCursor);
        //bellImageLabel->setVisible(true);
        bellNrLabel->setVisible(false);
    } else {
        //QPixmap image(":/resource/ico/" + events::getStyle() + "/mainDashBoard/bellNotify.png");
        //bellImageLabel->setPixmap(image);
        bellImageLabel->setFlat(true);
        bellImageLabel->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/bellNotify.png); border-radius: 0px");
        bellImageLabel->setCursor(Qt::PointingHandCursor);
        //bellImageLabel->setVisible(true);
        bellNrLabel->setVisible(true);
    }
    //bellImageLabel->setAlignment(Qt::AlignCenter);
    //bellImageLabel->setScaledContents(true);
    bellImageLabel->setGeometry(s(1141), s(48), s(45), s(42));

    bellNrLabel->setStyleSheet("color: white;");
    bellNrLabel->setText(QString::number(events::getBells()));
    bellNrLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    bellNrLabel->setAlignment(Qt::AlignCenter);
    bellNrLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    bellNrLabel->setGeometry(s(1141), s(48), s(45), s(42));
    bellNrLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
}

void mainboard::updateMessages() {
    if(events::getMessages() == 0) {
        QPixmap image(":/resource/ico/" + events::getStyle() + "/mainDashBoard/messageEmpty.png");
        messageImageLabel->setPixmap(image);
        messageNrLabel->setVisible(false);
    } else {
        QPixmap image(":/resource/ico/" + events::getStyle() + "/mainDashBoard/messageNotify.png");
        messageImageLabel->setPixmap(image);
        messageNrLabel->setVisible(true);
    }
    messageImageLabel->setAlignment(Qt::AlignCenter);
    messageImageLabel->setScaledContents(true);
    messageImageLabel->setGeometry(s(1183), s(48), s(45), s(42));

    messageNrLabel->setStyleSheet("color: white;");
    messageNrLabel->setText(QString::number(events::getMessages()));
    messageNrLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    messageNrLabel->setAlignment(Qt::AlignCenter);
    messageNrLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    messageNrLabel->setGeometry(s(1183), s(48), s(45), s(42));
}

void mainboard::updateUser() {
    QPixmap image(":/resource/ico/" + events::getStyle() + "/mainDashBoard/user.png");
    userLabel->setPixmap(image);
    userLabel->setAlignment(Qt::AlignCenter);
    userLabel->setScaledContents(true);
    userLabel->setGeometry(s(1144), s(23), s(14), s(22));
}

void mainboard::updateLogOut() {
    logOutButton->setGeometry(s(1250), s(60), s(75), s(30));
    logOutButton->setStyleSheet("background-color: " BUTON_COLOR_RED "; border-radius: " + QString::number((int)s(4)) + "px; color: #eee; ");
    logOutButton->setText(_tr("LOGOUT"));
    logOutButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.7)));
    logOutButton->setFlat(true);
    logOutButton->setCursor(Qt::PointingHandCursor);
}

void mainboard::updateObjects() {
    windowMain->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    windowMain->setWindowFlag(Qt::FramelessWindowHint, true);
    windowMain->setGeometry(0, 0, s(WINDOW_WIDTH), s(WINDOW_HEIGHT));

    mdiAreaHeader->setAttribute(Qt::WA_TranslucentBackground);
    mdiAreaHeader->setGeometry(0, 0, s(WINDOW_WIDTH), s(HEADER_HEIGHT));
    windowHeader->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    windowHeader->setWindowFlag(Qt::FramelessWindowHint, true);
    windowHeader->setGeometry(0, 0, s(WINDOW_WIDTH), s(HEADER_HEIGHT));
    windowHeader->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/header.png)");
    windowHeader->installEventFilter(this);

    mdiAreaLeftMenu->setAttribute(Qt::WA_TranslucentBackground);
    mdiAreaLeftMenu->setGeometry(0, s(HEADER_HEIGHT), s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
    windowLeftMenu->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    windowLeftMenu->setWindowFlag(Qt::FramelessWindowHint, true);
    windowLeftMenu->setGeometry(0, s(HEADER_HEIGHT), s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
    windowLeftMenu->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/sidebar.png)");
    windowLeftMenu->installEventFilter(this);
}

void mainboard::run() {
    if(pastNetworkConnection != events::getNetworkConnected() || pastNetwork != events::getRpcNetwork()) {
        pastNetworkConnection = events::getNetworkConnected();
        pastNetwork = events::getRpcNetwork();
        updateNetwork();
    }
    if(pastWalletNameKeyListChanged != events::getAccountNameKeyListChanged()) {
        pastWalletNameKeyListChanged = events::getAccountNameKeyListChanged();
        QString tmp = walletSelectorComboBox->currentText();
        walletSelectorComboBox->clear();
        walletSelectorComboBox->addItems(events::getAccountNameList());
        walletSelectorComboBox->setCurrentText(tmp);
        events::setSelectedNameKeyIndex(walletSelectorComboBox->currentIndex());
        updateWallet();
        on_Account_Changed(walletSelectorComboBox->currentText());
    }
    if(selectedNameKeyIndex != events::getSelectedNameKeyIndex()) {
        selectedNameKeyIndex = events::getSelectedNameKeyIndex();
        QString id = events::getAccountId(selectedNameKeyIndex);
        if(id.length()) {
            events::setId(id);
            walletSelectorComboBox->setCurrentIndex(selectedNameKeyIndex);
        }

    }
    if(pastBells != events::getBells()) {
        pastBells = events::getBells();
        updateBell();
    }
    if(pastMessages != events::getMessages()) {
        pastMessages = events::getMessages();
        updateMessages();
    }
    if(events::getShowTransitionsWindow()) {
        setState(STATE_TRANSITION);
    }
    if(currentState == STATE_DASHBOARD) {
        if(!dashboardWindow) {
            dashboardWindow = new dashboardpage();
            dashboardWindow->setVars(windowMain, parent);
        }
    }
    if((currentState == STATE_MY_WALLET || currentState == STATE_MY_WALLET_SEND || currentState == STATE_MY_WALLET_RECEIVE)) {
        if(!walletWindow) {
            walletWindow = new walletpage();
            walletWindow->setVars(windowMain, parent);
        }
    }
    if(currentState == STATE_TRANSITION) {
        if(!transitionsWindow) {
            transitionsWindow = new transitionswindow();
            transitionsWindow->setVars(windowMain, parent);
        }
    }
    if(currentState == STATE_SWAP) {
        if(!swapWindow) {
            swapWindow = new swapwindow();
            swapWindow->setVars(windowMain, parent);
        }
    }
    if(currentState == STATE_SEARCH) {
        if(!searchWindow) {
            searchWindow = new searchwindow();
            searchWindow->setVars(windowMain, parent);
        }
    }
    if(currentState == STATE_SETTINGS) {
        if(!settingsWindow) {
            settingsWindow = new settingswindow();
            settingsWindow->setVars(windowMain, parent);
        }
    }
    if(pastState != currentState || pastScale != events::getScale()  || pastLanguage.compare(translate::getCurrentLang())) {
        infoSwitch = false;
        if(pastState != currentState) {
            windowMain->setVisible(false);
        }

        if(currentState != STATE_NONE) {
            windowMain->setVisible(true);
            windowHeader->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/header.png)");
            windowLeftMenu->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/sidebar.png)");
        }
        if(dashboardWindow) {
            dashboardWindow->setState(dashboardpage::STATE_NONE);
        }
        if(swapWindow) {
            swapWindow->setState(swapwindow::STATE_NONE);
        }
        if(transitionsWindow) {
            transitionsWindow->setState(transitionswindow::STATE_NONE);
        }
        if(searchWindow) {
            searchWindow->setState(searchwindow::STATE_NONE);
        }
        if(settingsWindow) {
            settingsWindow->setState(settingswindow::STATE_NONE);
        }
        myWalletReceiveIcoLabel->setVisible(false);
        myWalletReceiveTextLabel->setVisible(false);
        myWalletReceiveButton->setVisible(false);

        myWalletSendIcoLabel->setVisible(false);
        myWalletSendTextLabel->setVisible(false);
        myWalletSendButton->setVisible(false);

        if(currentState == STATE_DASHBOARD) {
            walletSelectorLabel->setVisible(false);
            walletSelectorComboBox->setVisible(false);
            dashboardWindow->setState(dashboardpage::STATE_MAIN);
        } else if(currentState == STATE_MY_WALLET || currentState == STATE_MY_WALLET_SEND || currentState == STATE_MY_WALLET_RECEIVE) {
            walletSelectorLabel->setVisible(true);
            walletSelectorComboBox->setVisible(true);
            myWalletReceiveIcoLabel->setVisible(true);
            myWalletReceiveTextLabel->setVisible(true);
            myWalletReceiveButton->setVisible(true);

            myWalletSendIcoLabel->setVisible(true);
            myWalletSendTextLabel->setVisible(true);
            myWalletSendButton->setVisible(true);
        } else if(currentState == STATE_SWAP) {
            walletSelectorLabel->setVisible(true);
            walletSelectorComboBox->setVisible(true);
            if(swapWindow) {
                swapWindow->setState(swapwindow::STATE_SWAP);
            }
            parent->repaint();
        } else if(currentState == STATE_LYRA_TOKENS) {

        } else if(currentState == STATE_TRANSITION) {
            walletSelectorLabel->setVisible(true);
            walletSelectorComboBox->setVisible(true);
            if(transitionsWindow) {
                transitionsWindow->setState(transitionswindow::STATE_TRANSITIONS);
            }
            parent->repaint();
        } else if(currentState == STATE_SEARCH) {
            walletSelectorLabel->setVisible(false);
            walletSelectorComboBox->setVisible(false);
            if(searchWindow) {
                searchWindow->setState(searchwindow::STATE_SEARCH);
            }
        } else if(currentState == STATE_SETTINGS) {
            walletSelectorLabel->setVisible(false);
            walletSelectorComboBox->setVisible(false);
            settingsWindow->setState(settingswindow::STATE_SETTINGS);
        }

        if(walletWindow) {
            if(currentState == STATE_MY_WALLET)
                walletWindow->setState(walletpage::STATE_MAIN);
            else if(currentState == STATE_MY_WALLET_SEND)
                walletWindow->setState(walletpage::STATE_SEND);
            else if(currentState == STATE_MY_WALLET_RECEIVE)
                walletWindow->setState(walletpage::STATE_RECEIVE);
            else
                walletWindow->setState(walletpage::STATE_NONE);
        }
        if(getWalletUserPasswordChanged != events::getWalletUserPasswordChangedCnt()) {
            getWalletUserPasswordChanged = events::getWalletUserPasswordChangedCnt();
            userNameLabel->setText(events::getWalletUserPassword().first);
        }
        if(pastScale != events::getScale()) {
            updateNetwork();
            updateWallet();
            updateBell();
            updateMessages();
            updateUser();
            updateLogOut();
            updateObjects();
            updateMenuButtonsSize();
            userNameLabel->setGeometry(s(1168), s(22), s(150), s(25));
            userNameLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.0)));
            infoLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.0)));
        }

        if(pastLanguage.compare(translate::getCurrentLang())){
            updateLogOut();
            updateMenuButtonsLanguage();
            updateNetwork();
            updateWallet();
            userNameLabel->setText(events::getWalletUserPassword().first);
            userNameLabel->setFont(QFont(translate::getCurrentFontLight(), translate::getNumberFontSize(1.0)));
        }
        if(currentState != STATE_NONE) {
            updateMenuButtonsSize();
            updateMenuButtonsSelected();
        }
        windowMain->repaint();
        pastLanguage = translate::getCurrentLang();
        pastScale = events::getScale();
        pastState = currentState;
    }
    if(dashboardWindow) {
        dashboardWindow->run();
    }
    if(walletWindow) {
        walletWindow->run();
    }
    if(swapWindow) {
        swapWindow->run();
    }
    if(transitionsWindow) {
        transitionsWindow->run();
    }
    if(searchWindow) {
        searchWindow->run();
    }
    if(settingsWindow) {
        settingsWindow->run();
    }
}

void mainboard::on_logOut_ButtonPressed() {
    QApplication::closeAllWindows();
}

void mainboard::on_info_ButtonPressed() {
    infoWindow = new info();
    infoWindow->exec();
    delete infoWindow;
    infoWindow = nullptr;
}

bool mainboard::eventFilter(QObject *obj, QEvent *event) {
    /*if (obj == windowHeader) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (event->type() == QEvent::MouseButtonPress) {
            if(mouseEvent->button() == Qt::LeftButton){
               cursorPressPos= mouseEvent->pos();
               cursorIsMoving= true;
               event->accept();
               return false;
            }
       } else if(event->type() == QEvent::MouseMove && cursorIsMoving) {
           QPoint diff = mouseEvent->pos() - cursorPressPos;
           parent->window()->move(parent->window()->pos() + diff);
           return true;
       } else if(event->type() == QEvent::MouseButtonRelease) {
           cursorIsMoving= false;
           return false;
       } else {
           return false;
       }
   } else */if (obj == windowLeftMenu) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(event->type() == QEvent::MouseButtonRelease) {
            if(mouseEvent->button() == Qt::LeftButton) {
                if(mouseEvent->pos().x() > dashboardButton->geometry().x() &&
                    mouseEvent->pos().x() < dashboardButton->geometry().x() + dashboardButton->geometry().width() &&
                        mouseEvent->pos().y() > dashboardButton->geometry().y() &&
                            mouseEvent->pos().y() < dashboardButton->geometry().y() + dashboardButton->geometry().height()) {
                    currentState = STATE_DASHBOARD;
                } else if(mouseEvent->pos().x() > myWalletButton->geometry().x() &&
                    mouseEvent->pos().x() < myWalletButton->geometry().x() + myWalletButton->geometry().width() &&
                        mouseEvent->pos().y() > myWalletButton->geometry().y() &&
                            mouseEvent->pos().y() < myWalletButton->geometry().y() + myWalletButton->geometry().height()) {
                    currentState = STATE_MY_WALLET;
                } else if(mouseEvent->pos().x() > myWalletReceiveButton->geometry().x() &&
                    mouseEvent->pos().x() < myWalletReceiveButton->geometry().x() + myWalletReceiveButton->geometry().width() &&
                        mouseEvent->pos().y() > myWalletReceiveButton->geometry().y() &&
                            mouseEvent->pos().y() < myWalletReceiveButton->geometry().y() + myWalletReceiveButton->geometry().height()) {
                    currentState = STATE_MY_WALLET_RECEIVE;
                } else if(mouseEvent->pos().x() > myWalletSendButton->geometry().x() &&
                    mouseEvent->pos().x() < myWalletSendButton->geometry().x() + myWalletSendButton->geometry().width() &&
                        mouseEvent->pos().y() > myWalletSendButton->geometry().y() &&
                            mouseEvent->pos().y() < myWalletSendButton->geometry().y() + myWalletSendButton->geometry().height()) {
                    currentState = STATE_MY_WALLET_SEND;
                } else
#if LYRA_TOKEN_PAGE_IMPLEMENTED
                    if(mouseEvent->pos().x() > lyraTokenButton->geometry().x() &&
                    mouseEvent->pos().x() < lyraTokenButton->geometry().x() + lyraTokenButton->geometry().width() &&
                        mouseEvent->pos().y() > lyraTokenButton->geometry().y() &&
                            mouseEvent->pos().y() < lyraTokenButton->geometry().y() + lyraTokenButton->geometry().height()) {
                    currentState = STATE_LYRA_TOKENS;
                } else
#endif
#if SWAP_PAGE_IMPLEMENTED
                    if(mouseEvent->pos().x() > swapButton->geometry().x() &&
                    mouseEvent->pos().x() < swapButton->geometry().x() + swapButton->geometry().width() &&
                        mouseEvent->pos().y() > swapButton->geometry().y() &&
                            mouseEvent->pos().y() < swapButton->geometry().y() + swapButton->geometry().height()) {
                    currentState = STATE_SWAP;
                } else
#endif
#if TRANSITIONS_PAGE_IMPLEMENTED
                    if(mouseEvent->pos().x() > transactionsButton->geometry().x() &&
                    mouseEvent->pos().x() < transactionsButton->geometry().x() + transactionsButton->geometry().width() &&
                        mouseEvent->pos().y() > transactionsButton->geometry().y() &&
                            mouseEvent->pos().y() < transactionsButton->geometry().y() + transactionsButton->geometry().height()) {
                    currentState = STATE_TRANSITION;
                } else
#endif
#if SEARCH_PAGE_IMPLEMENTED
                    if(mouseEvent->pos().x() > searchButton->geometry().x() &&
                    mouseEvent->pos().x() < searchButton->geometry().x() + searchButton->geometry().width() &&
                        mouseEvent->pos().y() > searchButton->geometry().y() &&
                            mouseEvent->pos().y() < searchButton->geometry().y() + searchButton->geometry().height()) {
                    currentState = STATE_SEARCH;
                } else
#endif
                    if(mouseEvent->pos().x() > settingsButton->geometry().x() &&
                    mouseEvent->pos().x() < settingsButton->geometry().x() + settingsButton->geometry().width() &&
                        mouseEvent->pos().y() > settingsButton->geometry().y() &&
                            mouseEvent->pos().y() < settingsButton->geometry().y() + settingsButton->geometry().height()) {
                    currentState = STATE_SETTINGS;
                }
                run();
                return false;
            }
           return false;
        } else {
           return false;
        }
   } else {
        return false;
    }
    return false;
}

void mainboard::on_Account_Changed(const QString &) {
    events::setSelectedNameKeyIndex(walletSelectorComboBox->currentIndex());
    populate::refreshAll();
}

void mainboard::on_ring_pushed() {
    QPair<QString, int> event = events::getRingEvent();
    if(event.second == events::ringEvent_e::RING_EVENT_UNRECEIVED_BALANCE) {
        QStringList accNameList = events::getAccountNameList();
        QStringList split = event.first.split(":");
        int nr = accNameList.indexOf(split[0]);
        if(nr < 0)
            return;
        events::setSelectedNameKeyIndex(nr);
        currentState = STATE_MY_WALLET;
    }
}
