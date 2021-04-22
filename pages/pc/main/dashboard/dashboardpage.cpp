#include "dashboardpage.h"

#include <QMdiSubWindow>

#include "language/translate.h"
#include "wallet/events.h"

#define s(s) _scale(s)

dashboardpage::dashboardpage() {
}

dashboardpage::~dashboardpage() {
    mdiAreaDashboard->setVisible(false);
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

}

void dashboardpage::run() {
    if(pastState != currentState || pastScale != events::getScale()  || pastLanguage.compare(translate::getCurrentLang())) {

        mdiAreaDashboard->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
        windowDashboard->setGeometry(0, 0, mdiAreaDashboard->width(), mdiAreaDashboard->height());
        windowDashboard->setVisible(false);
        mdiAreaDashboard->setVisible(false);
        if(currentState == state_e::STATE_MAIN) {
            if(!dashMain) {
                dashMain = new dashmain(mdiAreaDashboard, windowDashboard);
            }
            windowDashboard->setVisible(true);
            mdiAreaDashboard->setVisible(true);
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

