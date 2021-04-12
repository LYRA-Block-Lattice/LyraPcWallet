#ifndef WALLETMAIN_H
#define WALLETMAIN_H

#include <QMainWindow>

#include <QEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QLabel>
#include <QPushButton>
#include <QDateEdit>
#include <QStandardItemModel>
#include <QtCharts/QChartView>
#include <QChart>
#include <QSplineSeries>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QCategoryAxis>

#include "config.h"
#include "wallet/events.h"

QT_CHARTS_USE_NAMESPACE

class walletmain : public QMainWindow
{
    Q_OBJECT
public:
    walletmain();
    ~walletmain();
    void setVars(QMdiArea *mdiArea = nullptr, QMdiSubWindow *parentWindow = nullptr);
    void run();

private:
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();

    QMdiSubWindow *parent;

    QLabel *myWalletLabel;

    QLabel *walletBalanceLabel;
    QLabel *walletBalanceValueLabel;
    QLabel *walletBalanceValueTotalLabel;

    QLabel *unreceivedBalanceLabel;
    QLabel *unreceivedBalanceValueLabel;
    QLabel *unreceivedBalanceValueTotalLabel;

    QPushButton *syncButton;

    QLabel *creationDateLabel;
    QLabel *creationDateDateLabel;
    QLabel *creationDateHourLabel;

    QLabel *numberOfTransactionsLabel;
    QLabel *numberOfTransactionsCountLabel;
    QLabel *numberOfTransactionsCountWeekLabel;

    QLabel *walletAddressLabel;
    QLabel *walletAddressIdLabel;

    QDateEdit *startBalanceDateEdit;
    QDateEdit *endBalanceDateEdit;

    QDateEdit *startValueDateEdit;
    QDateEdit *endValueDateEdit;


    QLabel *btcUsdLabel;
    QLabel *btcUsdSwitchLabel;
    bool usdSelected = true;

    QLabel *walletBalanceChartLabel;
    QLabel *walletValueChartLabel;

    QLineSeries *myWalletBalanceSeries;
    QChart *myWalletBalanceChart;
    QChartView *myWalletBalanceChartView;
    QDateTimeAxis *myWalletBalanceAxisX;
    QValueAxis *myWalletBalanceAxisY;

    QLineSeries *myWalletValueSeries;
    QChart *myWalletValueChart;
    QChartView *myWalletValueChartView;
    QDateTimeAxis *myWalletValueAxisX;
    QValueAxis *myWalletValueAxisY;

    double pastScale = 0.0;
    QString pastLanguage = "en";

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
};

#endif // WALLETMAIN_H
