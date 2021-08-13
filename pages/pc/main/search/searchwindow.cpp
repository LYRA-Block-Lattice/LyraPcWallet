
#include <QDesktopServices>

#include "searchwindow.h"
#include "language/translate.h"
#include "wallet/events.h"

#define s(s) _scale(s)

searchwindow::searchwindow() {

}

searchwindow::~searchwindow() {

}

void searchwindow::setVars(QMdiSubWindow *window, QWidget *parent) {
    this->setParent(parent);
    this->parent = parent;
    this->window = window;

    mdiAreaSearch = new QMdiArea(window);
    mdiAreaSearch->setAttribute(Qt::WA_TranslucentBackground);
    mdiAreaSearch->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
    mdiAreaSearch->setVisible(true);
    widgetSearch = new QWidget(mdiAreaSearch);
    widgetSearch->show();
    setParent(mdiAreaSearch);
    windowSearch = new QMdiSubWindow(mdiAreaSearch);
    windowSearch->setWidget(widgetSearch);
    windowSearch->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    windowSearch->setWindowFlag(Qt::FramelessWindowHint, true);
    windowSearch->setGeometry(0, 0, mdiAreaSearch->width(), mdiAreaSearch->height());
    windowSearch->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/settings/whiteBack.png)");
    windowSearch->setVisible(true);

    searchPushButton = new QPushButton(mdiAreaSearch);
    searchPushButton->setStyleSheet("border-image:url(:/resource/ico/" + events::getStyle() + "/mainDashBoard/wallet/receive/request.png); border-radius: 2px; border: 1px solid #eee; color: #fff; ");
    searchPushButton->setFlat(true);
    searchPushButton->setCursor(Qt::PointingHandCursor);
    searchPushButton->setVisible(true);
    connect(searchPushButton, SIGNAL(clicked()),this, SLOT(on_openNebula_ButtonPressed()));
}

void searchwindow::setState(state_e state) {
    currentState = state;
}

void searchwindow::refreshFonts() {
    searchPushButton->setFont(QFont(translate::getCurrentFontLight(), translate::getCurrentFontSizeLight(0.9)));
}

void searchwindow::refreshSize() {
    searchPushButton->setGeometry(s(350), s(30), s(330), s(39));
    refreshFonts();
}

void searchwindow::refreshLanguage() {
    searchPushButton->setText(_tr("OPEN NEBULA"));
    refreshFonts();
}

void searchwindow::run() {
    if(pastState != currentState) {
        pastState = currentState;
        if(currentState == STATE_SEARCH) {
            mdiAreaSearch->setVisible(true);
            windowSearch->setVisible(true);
        } else {
            mdiAreaSearch->setVisible(false);
            windowSearch->setVisible(false);
        }
    }
    if(pastScale != events::getScale()) {
        pastScale = events::getScale();
        mdiAreaSearch->setGeometry(s(LEFT_MENU_WIDTH), s(HEADER_HEIGHT), s(WINDOW_WIDTH) - s(LEFT_MENU_WIDTH), s(WINDOW_HEIGHT) - s(HEADER_HEIGHT));
        windowSearch->setGeometry(0, 0, mdiAreaSearch->width(), mdiAreaSearch->height());
        refreshSize();
    }
    if(pastLanguage.compare(translate::getCurrentLang())) {
        pastLanguage = translate::getCurrentLang();
        refreshLanguage();
    }
}

void searchwindow::on_openNebula_ButtonPressed() {
    QString link = (events::getNetwork() == events::network_e::NETWORK_MAINNET) ? "https://nebula.lyra.live" : "https://nebulatestnet.lyra.live";
    QDesktopServices::openUrl(QUrl(link));
}

