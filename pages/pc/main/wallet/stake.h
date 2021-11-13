#ifndef STAKE_H
#define STAKE_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTableView>
#include <QStandardItemModel>

#include "wallet/events.h"

class stake : public QMainWindow
{
    Q_OBJECT
public:
    typedef enum {
        NONE = 0,
        RUN
    }runMode_e;

    typedef enum {
        STATE_NONE = 0,
        STATE_CREATE_STACK_ACC,
        STATE_ADD_STAKING
    }state_e;
    stake();
    ~stake();
    void init(QMdiArea *mdiArea = nullptr, int accCnt = -1);
    void run();
    runMode_e getState();
    void setState(runMode_e state);

private:
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();
    void refreshStakingTable();
    QWidget *parent = nullptr;
    QMdiArea *thisMdiArea = nullptr;
    QWidget *thisWidget = nullptr;
    QMdiSubWindow *thisWindow = nullptr;

    QLabel *titleName = nullptr;
    QPushButton *refreshButton = nullptr;

    QLineEdit *accName = nullptr;
    QLineEdit *daysToStake = nullptr;
    QLineEdit *voteId = nullptr;

    QLabel *availableTokens = nullptr;
    QLineEdit *tokenAmount = nullptr;
    QPushButton *stakeOkButton = nullptr;


    QTableView *stakingAccTableView = nullptr;
    QStandardItemModel *stakingAccItemModel = nullptr;

    QPushButton *okButton = nullptr;
    QPushButton *closeButton = nullptr;

    runMode_e currentMode = NONE;
    runMode_e pastMode = NONE;
    state_e currentState = STATE_NONE;
    state_e pastState = STATE_NONE;

    int stakingAccountNr = -1;

    double pastScale = 0.0;
    QString pastLanguage = "";
    rpc::network_e network = rpc::network_e::NETWORK_TESTNET;

    int accCnt = -1;
private slots:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void createStakingAccount();
    void on_Ok_ButtonPressed();
    void on_StakeOk_ButtonPressed();
    void on_Refresh_ButtonPressed();
    void on_Close_ButtonPressed();
};

#endif // STAKE_H
