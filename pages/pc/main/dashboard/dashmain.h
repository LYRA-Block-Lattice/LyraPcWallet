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

    QMdiSubWindow *parent;

    QLabel *titleLabel;

    QLabel *totalSupplyLabel;
    QLabel *totalSupplyValueLabel;

    QLabel *teamLockedResetvedLabel;
    QLabel *teamLockedResetvedValueLabel;
    QLabel *teamLockedResetvedValueTotalLabel;

    QLabel *circulatingSupplyLabel;
    QLabel *circulatingSupplyValueLabel;
    QLabel *circulatingSupplyValueTotalLabel;

    QLabel *totalBlockLabel;
    QLabel *totalBlockCountLabel;
    //QLabel *totalBlockPercentageLabel;

    QLabel *myWalletValueLabel;
    QLabel *btcUsdLabel;
    QLabel *btcUsdSwitchLabel;
    bool usdSelected = true;
    QComboBox *chooseRangeComboBox;
    QLabel *orLabel;
    QLabel *startLabel;
    QDateEdit *startDateEdit;
    QLabel *endLabel;
    QDateEdit *endDateEdit;

    /*QSplineSeries*/QLineSeries *myWalletValueSeries;
    QChart *myWalletValueChart;
    QChartView *myWalletValueChartView;
    QDateTimeAxis *myWalletValueAxisX;
    QValueAxis *myWalletValueAxisY;

    QLabel *recentTransactionLabel;
    QPushButton *viewAllTransactionsButton;

    QLabel *assetsLabel;
    QPushButton *viewAllAssetsButton;

    QTableView *recentTransactionsTableView;
    QStandardItemModel *recentTransactionsItemModel;
    QTableView *assetsTableView;
    QStandardItemModel *assetsItemModel;


    double pastScale = 0.0;
    QString pastLanguage = "en";

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
