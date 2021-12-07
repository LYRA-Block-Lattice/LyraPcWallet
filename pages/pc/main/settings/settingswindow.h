#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>


#include "config.h"
#include "wallet/events.h"

#include "settingsaddaccount.h"
#include "settingsdeleteaccount.h"
#include "settingseditaccount.h"
#include "settingsedituser.h"
#include "settingsshowprivkey.h"
#include "createprofitaccount.h"


class settingswindow : public QMainWindow
{
    Q_OBJECT
public:
    typedef enum {
        STATE_NONE = 0,
        STATE_SETTINGS
    }state_e;

    settingswindow();
    ~settingswindow();
    void setVars(QMdiSubWindow *window = nullptr, QWidget *parent = nullptr);
    void run();
    void setState(state_e state);
    QMdiSubWindow *window = nullptr;

    settingsaddaccount *addAccountWindow = nullptr;
    settingsdeleteaccount *settingsDeleteAccount = nullptr;
    settingseditaccount *settingsEditAccount = nullptr;
    settingsedituser *settingsEditUser = nullptr;
    settingsshowprivkey *settingsShowPrivKey = nullptr;
    createprofitaccount *createProfitAccount = nullptr;
private:
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();
    void refreshStyle();
    void updateWalletSettingsTable();
    /* Window */
    QWidget *parent = nullptr;
    //QMdiArea *mdiArea;
    QMdiArea *mdiAreaSettings = nullptr;
    QWidget *widgetSettings = nullptr;
    QMdiSubWindow *windowSettings = nullptr;

    QLabel *generalSettingsLabel = nullptr;

    QLabel *languageLabel = nullptr;
    QComboBox *languageComboBox = nullptr;

    QLabel *appStyleLabel = nullptr;
    QComboBox *appStyleComboBox = nullptr;

    QLabel *networkLabel = nullptr;
    QComboBox *networkComboBox = nullptr;

    QLabel *alternativeValueLabel = nullptr;
    QComboBox *alternativeValueComboBox = nullptr;

    QLabel *windowScaleLabel = nullptr;
    QComboBox *windowScaleComboBox = nullptr;

    QLabel *customNodeIpLabel = nullptr;
    QLineEdit *customNodeIpLineEdit = nullptr;
    QPushButton *customNodeIpButton = nullptr;

    QLabel *userManagerSettingsLabel = nullptr;
    QLabel *userManagerSettingsBackgroundLabel = nullptr;
    QLabel *usernameLabel = nullptr;
    QLabel *usernameNameLabel = nullptr;
    QLabel *walletSettingsLabel = nullptr;
    QLabel *userPasswordLabel = nullptr;
    QLabel *userPasswordPassLabel = nullptr;
    QPushButton *backupButton = nullptr;
    QPushButton *editButton = nullptr;

    QTableView *accountsTableView = nullptr;
    QStandardItemModel *accountsItemModel = nullptr;




    state_e currentState = STATE_NONE;
    state_e pastState = STATE_SETTINGS;
    double pastScale = 0.0;
    QString pastLanguage = "";
    int walletNameKeyListChanged = -1;
    int styleCnt = -1;
    int getWalletUserPasswordChanged = -1;
    bool walletLoaded = false;
    bool udsSelected = false;
    rpc::network_e network = rpc::network_e::NETWORK_TESTNET;
    int customIpChanged = -1;

private slots:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void on_Backup_ButtonPressed();
    void on_Edit_ButtonPressed();
    void on_Add_ButtonPressed();
    void on_Language_Changed(const QString &);
    void on_AppStyle_Changed(const QString &);
    void on_Network_Changed(const QString &);
    void on_AlternativeValue_Changed(const QString &);
    void on_ScaleValue_Changed(const QString &);
    void on_CustomNodeIp_ButtonPressed();
};

#endif // SETTINGSWINDOW_H
