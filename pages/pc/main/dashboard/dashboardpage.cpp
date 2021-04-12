#include "dashboardpage.h"

#include <QMdiSubWindow>

#include "language/translate.h"
#include "wallet/events.h"

#define s(s) _scale(s)

dashboardpage::dashboardpage() {
}

dashboardpage::~dashboardpage() {
    delete dashMain;
}

void dashboardpage::setVars(QMdiSubWindow *window, QWidget *parent) {
    this->parent = parent;
    this->window = window;
    mdiAreaDashboard = new QMdiArea(window);
    mdiAreaDashboard->setAttribute(Qt::WA_TranslucentBackground);
    mdiAreaDashboard->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
    mdiAreaDashboard->setVisible(true);

    widget = new QWidget(window);
    widget->show();
    setParent(mdiAreaDashboard);

    windowDashboard = new QMdiSubWindow(mdiAreaDashboard);
    windowDashboard->setWidget(widget);
    windowDashboard->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    windowDashboard->setWindowFlag(Qt::FramelessWindowHint, true);
    windowDashboard->setGeometry(0, 0, mdiAreaDashboard->width(), mdiAreaDashboard->height());
    windowDashboard->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/dashboard/main/dashboard.png)");
    windowDashboard->setVisible(true);

    dashMain = new dashmain(mdiAreaDashboard, windowDashboard);

    mdiAreaRecentTransactions = new QMdiArea(window);
    mdiAreaRecentTransactions->setAttribute(Qt::WA_TranslucentBackground);
    mdiAreaRecentTransactions->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
    mdiAreaRecentTransactions->setVisible(false);
    windowRecentTransactions = new QMdiSubWindow(mdiAreaRecentTransactions);
    windowRecentTransactions->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    windowRecentTransactions->setWindowFlag(Qt::FramelessWindowHint, true);
    windowRecentTransactions->setGeometry(0, 0, mdiAreaRecentTransactions->width(), mdiAreaRecentTransactions->height());
    windowRecentTransactions->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/main_Light.png)");
    windowRecentTransactions->setVisible(false);


    mdiAreaAssets = new QMdiArea(window);
    mdiAreaAssets->setAttribute(Qt::WA_TranslucentBackground);
    mdiAreaAssets->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
    mdiAreaAssets->setVisible(false);
    windowAssets = new QMdiSubWindow(mdiAreaAssets);
    windowAssets->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    windowAssets->setWindowFlag(Qt::FramelessWindowHint, true);
    windowAssets->setGeometry(0, 0, mdiAreaAssets->width(), mdiAreaAssets->height());
    windowAssets->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/main_Light.png)");
    windowAssets->setVisible(false);
}

void dashboardpage::run() {
    if(pastState != currentState || pastScale != events::getScale()  || pastLanguage.compare(translate::getCurrentLang())) {

        mdiAreaDashboard->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
        windowDashboard->setGeometry(0, 0, mdiAreaDashboard->width(), mdiAreaDashboard->height());
        mdiAreaRecentTransactions->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
        windowRecentTransactions->setGeometry(0, 0, mdiAreaDashboard->width(), mdiAreaDashboard->height());
        mdiAreaAssets->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
        windowAssets->setGeometry(0, 0, mdiAreaDashboard->width(), mdiAreaDashboard->height());

        windowDashboard->setVisible(false);
        mdiAreaDashboard->setVisible(false);
        windowRecentTransactions->setVisible(false);
        mdiAreaRecentTransactions->setVisible(false);
        windowAssets->setVisible(false);
        mdiAreaAssets->setVisible(false);
        if(currentState == state_e::STATE_MAIN) {
            windowDashboard->setVisible(true);
            mdiAreaDashboard->setVisible(true);
        } else if(currentState == state_e::STATE_RECENT_TRANSACTIONS) {
            windowRecentTransactions->setVisible(true);
            mdiAreaRecentTransactions->setVisible(true);
        } else if(currentState == state_e::STATE_ASSETS) {
            windowAssets->setVisible(true);
            mdiAreaAssets->setVisible(true);
        }
        pastLanguage = translate::getCurrentLang();
        pastScale = events::getScale();
        pastState = currentState;
    }
    dashMain->run();
}

void dashboardpage::setState(state_e state) {
    currentState = state;
}

