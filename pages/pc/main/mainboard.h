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



    /* Window */
    QWidget *parent;
    QMdiArea *mdiArea;
    QMdiSubWindow *windowMain;
    //QMdiArea mdiAreaMain;
    QMdiArea *mdiAreaHeader;
    QWidget *widgetHeader;
    QMdiSubWindow *windowHeader;
    QMdiArea *mdiAreaLeftMenu;
    QWidget *widgetLeftMenu;
    QMdiSubWindow *windowLeftMenu;

    dashboardpage *dashboardWindow;
    walletpage *walletWindow;
    settingswindow *settingsWindow;
    transitionswindow *transitionsWindow;

    QLabel *userNameLabel;

    QLabel *walletSelectorLabel;
    QComboBox *walletSelectorComboBox;
    QLabel *networkLabelImage;
    QLabel *networkLabel;
    QLabel *bellImageLabel;
    QLabel *bellNrLabel;
    QLabel *messageImageLabel;
    QLabel *messageNrLabel;
    QLabel *userLabel;
    QPushButton *logOutButton;

    QLabel *dashboardIcoLabel;
    QLabel *dashboardTextLabel;
    QLabel *dashboardArowLabel;
    QLabel *dashboardButton;

    QLabel *myWalletIcoLabel;
    QLabel *myWalletTextLabel;
    QLabel *myWalletArowLabel;
    QLabel *myWalletButton;

    QLabel *myWalletReceiveIcoLabel;
    QLabel *myWalletReceiveTextLabel;
    QLabel *myWalletReceiveButton;

    QLabel *myWalletSendIcoLabel;
    QLabel *myWalletSendTextLabel;
    QLabel *myWalletSendButton;

    QLabel *lyraTokensIcoLabel;
    QLabel *lyraTokensTextLabel;
    QLabel *lyraTokensArowLabel;
    QLabel *lyraTokenButton;

    QLabel *swapIcoLabel;
    QLabel *swapTextLabel;
    QLabel *swapArowLabel;
    QLabel *swapButton;

    QLabel *transactionsIcoLabel;
    QLabel *transactionsTextLabel;
    QLabel *transactionsArowLabel;
    QLabel *transactionsButton;

    QLabel *searchIcoLabel;
    QLabel *searchTextLabel;
    QLabel *searchArowLabel;
    QLabel *searchButton;

    QLabel *settingsIcoLabel;
    QLabel *settingsTextLabel;
    QLabel *settingsArowLabel;
    QLabel *settingsButton;


    state_e currentState = STATE_NONE;
    state_e pastState = STATE_NONE;
    double pastScale = DEFAULT_SCALE;
    QString pastLanguage = "en";
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
    void on_Account_Changed(const QString &);
};

#endif // MAIN_H
