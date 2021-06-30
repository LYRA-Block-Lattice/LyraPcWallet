#ifndef DASHMAIN_H
#define DASHMAIN_H

#include <QEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>
#include <QTableView>
#include <QStandardItemModel>
#include <QtCharts/QChartView>
#include <QChart>
#include <QSplineSeries>
#include <QGraphicsView>
#include <QDateTimeAxis>
#include <QCategoryAxis>

#include "config.h"
#include "wallet/events.h"

QT_CHARTS_USE_NAMESPACE

class dashmain : public QMainWindow
{
public:
    dashmain(QMdiArea *mdiArea = nullptr, QMdiSubWindow *parentWindow = nullptr);
    ~dashmain();
    void run();
private:
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();
    void updateLastTransactionsTables();
    void updateAssetsTables();

    QMdiSubWindow *parent = nullptr;

    QLabel *titleLabel = nullptr;

    QLabel *totalSupplyLabel = nullptr;
    QLabel *totalSupplyValueLabel = nullptr;

    QLabel *teamLockedResetvedLabel = nullptr;
    QLabel *teamLockedResetvedValueLabel = nullptr;
    QLabel *teamLockedResetvedValueTotalLabel = nullptr;

    QLabel *circulatingSupplyLabel = nullptr;
    QLabel *circulatingSupplyValueLabel = nullptr;
    QLabel *circulatingSupplyValueTotalLabel = nullptr;

    QLabel *totalBlockLabel = nullptr;
    QLabel *totalBlockCountLabel = nullptr;
    //QLabel *totalBlockPercentageLabel = nullptr;

    QLabel *myWalletValueLabel = nullptr;
    QLabel *btcUsdLabel = nullptr;
    QLabel *btcUsdSwitchLabel = nullptr;
    bool usdSelected = true;
    QComboBox *chooseRangeComboBox = nullptr;
    QLabel *orLabel = nullptr;
    QLabel *startLabel = nullptr;
    QDateEdit *startDateEdit = nullptr;
    QLabel *endLabel = nullptr;
    QDateEdit *endDateEdit = nullptr;

    /*QSplineSeries*/QLineSeries *myWalletValueSeries = nullptr;
    QChart *myWalletValueChart = nullptr;
    QChartView *myWalletValueChartView = nullptr;
    QDateTimeAxis *myWalletValueAxisX = nullptr;
    QValueAxis *myWalletValueAxisY = nullptr;

    QLabel *recentTransactionLabel = nullptr;
    QPushButton *viewAllTransactionsButton = nullptr;

    QLabel *assetsLabel = nullptr;
    QPushButton *viewAllAssetsButton = nullptr;

    QTableView *recentTransactionsTableView = nullptr;
    QStandardItemModel *recentTransactionsItemModel = nullptr;
    QTableView *assetsTableView = nullptr;
    QStandardItemModel *assetsItemModel = nullptr;


    double pastScale = 0.0;
    QString pastLanguage = "";

    qint64 totalSupply = 1;
    qint64 teamLockedReserved = 1;
    qint64 circulatingSupply = 1;
    qint64 totalBlockCount = 1;
    qint64 minimumDateTime = 1;
    int myWalletValueAllChartModifyedCnt = 1;
    int recentTransactionsListModifyedCnt = 1;
    int assetsListModifyedCnt = 1;
    int dateRangeModifyedCnt = 1;
private slots:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void on_StartDate_EditFinished();
    void on_EndDate_EditFinished();
    void on_ViewAllTransactions_ButtonPushed();
    void on_ViewAllAssets_ButtonPushed();
};

#endif // DASHMAIN_H
