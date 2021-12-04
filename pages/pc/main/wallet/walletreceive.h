#ifndef WALLETRECEIVE_H
#define WALLETRECEIVE_H

#include <QMainWindow>


#include <QEvent>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTableView>
#include <QStandardItemModel>
#include <QGraphicsScene>
#include <QPixmap>
#include <QImage>
#include <QComboBox>

#include "config.h"
#include "wallet/events.h"
#include "walletshowdetails.h"

class walletreceive : public QMainWindow
{
    Q_OBJECT
public:
    walletreceive();
    ~walletreceive();
    void setVars(QMdiArea *mdiArea = nullptr, QMdiSubWindow *parentWindow = nullptr);
    void run();

    walletshowdetails *showDetailsWindow;
private:
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();
    void refreshStyle();
    void updateReceived();
    void showDetails(int order);

    QTimer refreshTotalReceivedLastWeekTimer;

    QMdiSubWindow *parent;
    QMdiArea *mdiArea;
    QLabel *myWalletLabel;

    QLabel *walletBalanceLabel;
    QLabel *walletBalanceValueLabel;
    QLabel *walletBalanceValueTotalLabel;

    QLabel *unreceivedBalanceLabel;
    QLabel *unreceivedBalanceValueLabel;
    QLabel *unreceivedBalanceValueTotalLabel;

    QPushButton *syncButton;

    QLabel *totalLyrReceivedLabel;
    QLabel *totalLyrReceivedValueLabel;
    QLabel *totalLyrReceivedLastWeekLabel;

    QLabel *lastReceivedLabel;
    QLabel *lastReceivedDateLabel;
    QLabel *lastReceivedValueLabel;

    QLabel *walletAddressLabel;
    QLabel *walletAddressIdLabel;

    QLabel *requestToWalletIdLabel;
    QLineEdit *requestToWalletIdLineEdit;
    QLabel *requestToWalletIdStatusLabel;

    QLabel *ammountLabel;
    QComboBox *tokenComboBox;
    //QLabel *lyrTextLabel;
    QLineEdit *ammountLineEdit;
    QLabel *ammountValueStatusLabel;
    QLabel *btcUsdTextLabel;
    QLabel *ammountApproximateLabel;
    QLineEdit *btcUsdLineEdit;
    QLabel *btcUsdStatusLabel;

    QLabel *qrCodeLabel;
    QLabel *qrCodeImageLabel;

    QPushButton *requestButton;

    QLabel *btcUsdLabel;
    QLabel *btcUsdSwitchLabel;
    bool usdSelected = true;

    QLabel *lastReceivedTransitionsArrowLabel;
    QLabel *lastReceivedTransitionsLabel;

    QTableView *recentTransactionsTableView;
    QStandardItemModel *recentTransactionsItemModel;


    double pastScale = 0.0;
    QString pastLanguage = "";

    double ballance = 1.0;
    QString unreceivedBallance;
    double percentageOfTotalSupply = 1.0;
    qint64 lastSyncDateTime = -1;
    double totalLyraReceivedLastWeek = -1;
    double totalLyraPercentageReceivedLastWeek = -1;
    int lastReceivedTransactionsListModifyedCnt = -1;
    int recentRxTransactionsModifyedCnt = -1;

    bool userInputSemaphore = false;

    int idModifyedCnt = -1;
    int tokenListModifyedCnt = -1;
private slots:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void on_requestToWalletIdStatusLabel_Changed(const QString &str);
    void on_destinationWalletIdLineEdit_Changed(const QString &str);
    void on_tokenComboBox_TextChanget(const QString &);
    void on_ammountLineEdit_Changed(const QString &);
    void on_ammountUsdBtcLineEdit_Changed(const QString &);
    void on_Sync_ButtonPressed();
    void on_RefreshTotalReceivedLastWeekTimer();
};

#endif // WALLETRECEIVE_H
