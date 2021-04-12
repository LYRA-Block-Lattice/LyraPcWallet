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
private:
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();
    void updateWalletSettingsTable();
    /* Window */
    QWidget *parent;
    QMdiSubWindow *window;
    //QMdiArea *mdiArea;
    QMdiArea *mdiAreaSettings;
    QWidget *widgetSettings;
    QMdiSubWindow *windowSettings;

    QLabel *generalSettingsLabel;

    QLabel *languageLabel;
    QComboBox *languageComboBox;

    QLabel *appStyleLabel;
    QComboBox *appStyleComboBox;

    QLabel *networkLabel;
    QComboBox *networkComboBox;

    QLabel *alternativeValueLabel;
    QComboBox *alternativeValueComboBox;

    QLabel *windowScaleLabel;
    QComboBox *windowScaleComboBox;

    QLabel *userManagerSettingsLabel;
    QLabel *userManagerSettingsBackgroundLabel;
    QLabel *usernameLabel;
    QLabel *usernameNameLabel;
    QLabel *walletSettingsLabel;
    QLabel *userPasswordLabel;
    QLabel *userPasswordPassLabel;
    QPushButton *backupButton;
    QPushButton *editButton;

    QTableView *accountsTableView;
    QStandardItemModel *accountsItemModel;


    settingsaddaccount *addAccountWindow;
    settingsdeleteaccount *settingsDeleteAccount;
    settingseditaccount *settingsEditAccount;
    settingsedituser *settingsEditUser;
    settingsshowprivkey *settingsShowPrivKey;


    state_e currentState = STATE_NONE;
    state_e pastState = STATE_SETTINGS;
    double pastScale = 0.0;
    QString pastLanguage = "en";
    int walletNameKeyListChanged = -1;
    int styleCnt = -1;
    int getWalletUserPasswordChanged = -1;
    bool walletLoaded = false;
    bool udsSelected = false;
    rpc::network_e network = rpc::network_e::NETWORK_TESTNET;

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
};

#endif // SETTINGSWINDOW_H
