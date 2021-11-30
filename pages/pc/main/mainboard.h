#ifndef MAIN_H
#define MAIN_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QWidget>
#include <QMdiArea>
#include <QLabel>
#include <QComboBox>
#include <QGraphicsScene>
#include <QPushButton>

#include "config.h"
#include "wallet/events.h"

#include "dashboard/dashboardpage.h"
#include "wallet/walletpage.h"
#include "settings/settingswindow.h"
#include "transitions/transitionswindow.h"
#include "search/searchwindow.h"
#include "swap/swapwindow.h"

#include "info.h"

class mainboard : public QMainWindow {
    Q_OBJECT
public:
    typedef enum {
        STATE_NONE = 0,
        STATE_DASHBOARD,
        STATE_MY_WALLET,
        STATE_MY_WALLET_SEND,
        STATE_MY_WALLET_RECEIVE,
        STATE_LYRA_TOKENS,
        STATE_SWAP,
        STATE_TRANSITION,
        STATE_SEARCH,
        STATE_SETTINGS
    }state_e;

    typedef enum {
        CMD_NONE = 0,
        CMD_LOGOUT,
    }command_e;

    mainboard();
    ~mainboard();
    void setVars(QMdiArea *mdiArea = nullptr, QWidget *parent = nullptr);
    void run();
    command_e getCommand();
    void setState(state_e state);

    /* Window */
    QWidget *parent = nullptr;
    QMdiArea *mdiArea = nullptr;
    QMdiSubWindow *windowMain = nullptr;
private:
    void updateWallet();
    void updateNetwork();
    void updateBell();
    void updateMessages();
    void updateUser();
    void updateLogOut();
    void updateObjects();

    void updateMenuButtonsFont();
    void updateMenuButtonsSize();
    void updateMenuButtonsLanguage();
    void updateMenuButtonsSelected();



    //QMdiArea mdiAreaMain;
    QMdiArea *mdiAreaHeader = nullptr;
    QWidget *widgetHeader = nullptr;
    QMdiSubWindow *windowHeader = nullptr;
    QMdiArea *mdiAreaLeftMenu = nullptr;
    QWidget *widgetLeftMenu = nullptr;
    QMdiSubWindow *windowLeftMenu = nullptr;

    dashboardpage *dashboardWindow = nullptr;
    walletpage *walletWindow = nullptr;
    swapwindow *swapWindow = nullptr;
    transitionswindow *transitionsWindow = nullptr;
    searchwindow *searchWindow = nullptr;
    settingswindow *settingsWindow = nullptr;

    info *infoWindow = nullptr;

    QLabel *userNameLabel = nullptr;

    QLabel *walletSelectorLabel = nullptr;
    QComboBox *walletSelectorComboBox = nullptr;
    QLabel *networkLabelImage = nullptr;
    QLabel *networkLabel = nullptr;
    QPushButton *bellImageLabel = nullptr;
    QLabel *bellNrLabel = nullptr;
    QLabel *messageImageLabel = nullptr;
    QLabel *messageNrLabel = nullptr;
    QLabel *userLabel = nullptr;
    QPushButton *logOutButton = nullptr;

    QLabel *dashboardIcoLabel = nullptr;
    QLabel *dashboardTextLabel = nullptr;
    QLabel *dashboardArowLabel = nullptr;
    QLabel *dashboardButton = nullptr;

    QLabel *myWalletIcoLabel = nullptr;
    QLabel *myWalletTextLabel = nullptr;
    QLabel *myWalletArowLabel = nullptr;
    QLabel *myWalletButton = nullptr;

    QLabel *myWalletReceiveIcoLabel = nullptr;
    QLabel *myWalletReceiveTextLabel = nullptr;
    QLabel *myWalletReceiveButton = nullptr;

    QLabel *myWalletSendIcoLabel = nullptr;
    QLabel *myWalletSendTextLabel = nullptr;
    QLabel *myWalletSendButton = nullptr;

    QLabel *lyraTokensIcoLabel = nullptr;
    QLabel *lyraTokensTextLabel = nullptr;
    QLabel *lyraTokensArowLabel = nullptr;
    QLabel *lyraTokenButton = nullptr;

    QLabel *swapIcoLabel = nullptr;
    QLabel *swapTextLabel = nullptr;
    QLabel *swapArowLabel = nullptr;
    QLabel *swapButton = nullptr;

    QLabel *transactionsIcoLabel = nullptr;
    QLabel *transactionsTextLabel = nullptr;
    QLabel *transactionsArowLabel = nullptr;
    QLabel *transactionsButton = nullptr;

    QLabel *searchIcoLabel = nullptr;
    QLabel *searchTextLabel = nullptr;
    QLabel *searchArowLabel = nullptr;
    QLabel *searchButton = nullptr;

    QLabel *settingsIcoLabel = nullptr;
    QLabel *settingsTextLabel = nullptr;
    QLabel *settingsArowLabel = nullptr;
    QLabel *settingsButton = nullptr;

    QLabel *infoLabel = nullptr;
    QPushButton *infoButton = nullptr;


    state_e currentState = STATE_NONE;
    state_e pastState = STATE_NONE;
    double pastScale = 0.0;
    QString pastLanguage = "";
    bool pastNetworkConnection = false;
    int pastWalletNameKeyListChanged = -1;
    int selectedNameKeyIndex = -1;
    int getWalletUserPasswordChanged = -1;
    int pastBells = 0;
    int pastMessages = 0;
    events::network_e pastNetwork = events::network_e::DEFAULT_NETWORK;
    bool infoSwitch = false;
    command_e command = CMD_NONE;
    QPoint cursorPressPos;
    bool cursorIsMoving{false};
    bool networkConnected = false;

private slots:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void on_logOut_ButtonPressed();
    void on_info_ButtonPressed();
    void on_Account_Changed(const QString &);
    void on_ring_pushed();
};

#endif // MAIN_H
