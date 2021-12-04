#ifndef WALLETMAIN_H
#define WALLETMAIN_H

#include <QMainWindow>

#include <QEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QLabel>
#include <QPushButton>
#include <QDateEdit>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QtCharts/QChartView>
#include <QChart>
#include <QSplineSeries>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QCategoryAxis>

#include "stake.h"

#include "config.h"
#include "wallet/events.h"

QT_CHARTS_USE_NAMESPACE

class walletmain : public QMainWindow
{
    Q_OBJECT
public:
    typedef enum {
        STATE_NONE = 0,
        STATE_WALLET
    }state_e;

    walletmain();
    ~walletmain();
    void setVars(QMdiArea *mdiArea = nullptr, QMdiSubWindow *parentWindow = nullptr);
    void run();
    void resetWindow();

private:
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();
    void refreshStyle();

    QMdiSubWindow *parent = nullptr;
    QMdiArea *mdiArea = nullptr;

    QLabel *myWalletLabel = nullptr;

    QLabel *walletBalanceLabel = nullptr;
    QLabel *walletBalanceValueLabel = nullptr;
    QLabel *walletBalanceValueTotalLabel = nullptr;

    QLabel *unreceivedBalanceLabel = nullptr;
    QLabel *unreceivedBalanceValueLabel = nullptr;
    QLabel *unreceivedBalanceValueTotalLabel = nullptr;

    QPushButton *syncButton = nullptr;

    QLabel *creationDateLabel = nullptr;
    QLabel *creationDateDateLabel = nullptr;
    QLabel *creationDateHourLabel = nullptr;

    QLabel *numberOfTransactionsLabel = nullptr;
    QLabel *numberOfTransactionsCountLabel = nullptr;
    QLabel *numberOfTransactionsCountWeekLabel = nullptr;

    QLabel *walletAddressLabel = nullptr;
    QLabel *walletAddressIdLabel = nullptr;

    QPushButton *voteUnvoteButton = nullptr;

    QDateEdit *startBalanceDateEdit = nullptr;
    QDateEdit *endBalanceDateEdit = nullptr;

    QDateEdit *startValueDateEdit = nullptr;
    QDateEdit *endValueDateEdit = nullptr;


    QLabel *btcUsdLabel = nullptr;
    QLabel *btcUsdSwitchLabel = nullptr;
    bool usdSelected = true;

    QLabel *walletBalanceChartLabel = nullptr;
    QLabel *walletValueChartLabel = nullptr;

    QLineSeries *myWalletBalanceSeries = nullptr;
    QChart *myWalletBalanceChart = nullptr;
    QChartView *myWalletBalanceChartView = nullptr;
    QDateTimeAxis *myWalletBalanceAxisX = nullptr;
    QValueAxis *myWalletBalanceAxisY = nullptr;

    QLineSeries *myWalletValueSeries = nullptr;
    QChart *myWalletValueChart = nullptr;
    QChartView *myWalletValueChartView = nullptr;
    QDateTimeAxis *myWalletValueAxisX = nullptr;
    QValueAxis *myWalletValueAxisY = nullptr;

    stake *stakeWindow = nullptr;

    state_e currentState = STATE_WALLET;
    state_e pastState = STATE_NONE;
    double pastScale = 0.0;
    QString pastLanguage = "";

    double ballance = 1.0;
    QString unreceivedBallance;
    double percentageOfTotalSupply = 1.0;
    qint64 creationDateTime = -1;
    qint64 lastSyncDateTime = -1;
    int nrOfTransactions = -1;
    int nrOfTransactionsLastWeek = -1;
    qint64 minimumDateTime = -1;
    int myWalletBalanceChartModifyedCnt = -1;
    int myWalletValueChartModifyedCnt = -1;
    int dateRangeModifyedCnt = -1;
    int idModifyedCnt = -1;

    bool userInputSemaphore = false;

private slots:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void on_StartDateBalance_EditFinished();
    void on_EndDateBalance_EditFinished();
    void on_StartDateValue_EditFinished();
    void on_EndDateValue_EditFinished();
    void on_Sync_ButtonPressed();
    void on_VoteUnvote_ButtonPressed();
};

#endif // WALLETMAIN_H
