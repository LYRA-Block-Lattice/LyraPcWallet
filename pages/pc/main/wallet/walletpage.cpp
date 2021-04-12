#include "walletpage.h"

#include <QMdiSubWindow>

#include "language/translate.h"
#include "wallet/events.h"

#define s(s) _scale(s)

walletpage::walletpage() {
}

walletpage::~walletpage() {
    /*delete windowReceive;
    windowReceive = nullptr;
    delete windowSend;
    windowSend = nullptr;
    delete windowWallet;
    windowWallet= nullptr;*/
}

void walletpage::setVars(QMdiSubWindow *window, QWidget *parent) {
    this->parent = parent;
    this->window = window;
    mdiAreaWallet = new QMdiArea(window);
    mdiAreaWallet->setAttribute(Qt::WA_TranslucentBackground);
    mdiAreaWallet->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
    mdiAreaWallet->setVisible(false);
    widgetWallet = new QWidget(window);
    widgetWallet->show();
    setParent(mdiAreaWallet);
    windowWallet = new QMdiSubWindow(mdiAreaWallet);
    windowWallet->setWidget(widgetWallet);
    windowWallet->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    windowWallet->setWindowFlag(Qt::FramelessWindowHint, true);
    windowWallet->setGeometry(0, 0, mdiAreaWallet->width(), mdiAreaWallet->height());
    windowWallet->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/main/main.png)");
    windowWallet->setVisible(false);

    walletMain = new walletmain();
    walletMain->setVars(mdiAreaWallet, windowWallet);

    mdiAreaSend = new QMdiArea(window);
    mdiAreaSend->setAttribute(Qt::WA_TranslucentBackground);
    mdiAreaSend->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
    mdiAreaSend->setVisible(false);
    widgetSend = new QWidget(window);
    widgetSend->show();
    windowSend = new QMdiSubWindow(mdiAreaSend);
    windowSend->setWidget(widgetSend);
    windowSend->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    windowSend->setWindowFlag(Qt::FramelessWindowHint, true);
    windowSend->setGeometry(0, 0, mdiAreaSend->width(), mdiAreaSend->height());
    windowSend->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/send/send.png)");
    windowSend->setVisible(false);

    walletSend = new walletsend();
    walletSend->setVars(mdiAreaSend, windowSend);

    mdiAreaReceive = new QMdiArea(window);
    mdiAreaReceive->setAttribute(Qt::WA_TranslucentBackground);
    mdiAreaReceive->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
    mdiAreaReceive->setVisible(false);
    widgetReceive = new QWidget(window);
    widgetReceive->show();
    windowReceive = new QMdiSubWindow(mdiAreaReceive);
    windowReceive->setWidget(widgetReceive);
    windowReceive->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    windowReceive->setWindowFlag(Qt::FramelessWindowHint, true);
    windowReceive->setGeometry(0, 0, mdiAreaReceive->width(), mdiAreaReceive->height());
    windowReceive->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/receive/receive.png)");
    windowReceive->setVisible(false);

    walletReceive = new walletreceive();
    walletReceive->setVars(mdiAreaReceive, windowReceive);

}

void walletpage::run() {
    if(pastState != currentState) {
        walletSend->showDetailsWindow->setDetailsVisible(false);
        walletReceive->showDetailsWindow->setDetailsVisible(false);
    }
    if(pastState != currentState || pastScale != events::getScale()  || pastLanguage.compare(translate::getCurrentLang())) {

        mdiAreaWallet->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
        windowWallet->setGeometry(0, 0, mdiAreaWallet->width(), mdiAreaWallet->height());
        mdiAreaSend->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
        windowSend->setGeometry(0, 0, mdiAreaWallet->width(), mdiAreaWallet->height());
        mdiAreaReceive->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
        windowReceive->setGeometry(0, 0, mdiAreaWallet->width(), mdiAreaWallet->height());

        windowWallet->setVisible(false);
        mdiAreaWallet->setVisible(false);
        windowSend->setVisible(false);
        mdiAreaSend->setVisible(false);
        windowReceive->setVisible(false);
        mdiAreaReceive->setVisible(false);
        if(currentState == state_e::STATE_MAIN) {
            windowWallet->setVisible(true);
            mdiAreaWallet->setVisible(true);
        } else if(currentState == state_e::STATE_SEND) {
            windowSend->setVisible(true);
            mdiAreaSend->setVisible(true);
        } else if(currentState == state_e::STATE_RECEIVE) {
            windowReceive->setVisible(true);
            mdiAreaReceive->setVisible(true);
        }
        pastLanguage = translate::getCurrentLang();
        pastScale = events::getScale();
        pastState = currentState;
    }
    walletMain->run();
    walletReceive->run();
    walletSend->run();
}

void walletpage::setState(state_e state) {
    currentState = state;
}
