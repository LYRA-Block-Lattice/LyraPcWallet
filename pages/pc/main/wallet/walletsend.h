#ifndef WALLETSEND_H
#define WALLETSEND_H

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

class walletsend : public QMainWindow
{
    Q_OBJECT
public:
    walletsend();
    ~walletsend();
    void setVars(QMdiArea *mdiArea = nullptr, QMdiSubWindow *parentWindow = nullptr);
    void run();

    walletshowdetails *showDetailsWindow;

private:
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();
    void updateSend();
    void showDetails(int order);

    QTimer refreshTotalSendedLastWeekTimer;

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

    QLabel *totalLyrSendedLabel;
    QLabel *totalLyrSendedValueLabel;
    QLabel *totalLyrSendedLastWeekLabel;

    QLabel *lastSendLabel;
    QLabel *lastSendDateLabel;
    QLabel *lastSendValueLabel;

    QLabel *walletAddressLabel;
    QLabel *walletAddressIdLabel;

    QLabel *destinationWalletIdLabel;
    QLineEdit *destinationWalletIdLineEdit;
    QLabel *destinationWalletIdStatusLabel;

    QLabel *ammountLabel;
    QComboBox *tokenComboBox;
    QLineEdit *ammountLineEdit;
    QLabel *ammountValueStatusLabel;
    QLabel *ammountApproximateLabel;
    QLabel *btcUsdTextLabel;
    QLineEdit *btcUsdLineEdit;
    QLabel *btcUsdStatusLabel;

    QLabel *btcUsdLabel;
    QLabel *btcUsdSwitchLabel;
    bool usdSelected = true;

    QLabel *passwordConfirmationLabel;
    QLineEdit *passwordConfirmationLineEdit;
    QLabel *passwordConfirmationStatusLabel;

    QLabel *noteLabel;
    QLineEdit *noteLineEdit;

    QPushButton *sendButton;

    QLabel *lastSendedTransitionsArrowLabel;
    QLabel *lastSendedTransitionsLabel;

    QTableView *recentTransactionsTableView;
    QStandardItemModel *recentTransactionsItemModel;

    double pastScale = 0.0;
    QString pastLanguage = "en";

    double ballance = 1.0;
    QString unreceivedBallance;
    double percentageOfTotalSupply = 1.0;
    qint64 lastSyncDateTime = -1;
    double totalLyraSendedLastWeek = -1;
    double totalLyraPercentageSendedLastWeek = -1;
    int lastSendedTransactionsListModifyedCnt = -1;
    int recentTxTransactionsModifyedCnt = -1;

    bool userInputSemaphore = false;

    int idModifyedCnt = -1;
    int tokenListModifyedCnt = -1;
private slots:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void on_destinationWalletIdLineEdit_Changed(const QString &);
    void on_tokenComboBox_TextChanget(const QString &);
    void on_ammountLineEdit_Changed(const QString &);
    void on_ammountUsdBtcLineEdit_Changed(const QString &);
    void on_PasswordLineEdit_Changed(const QString &);
    void on_Sync_ButtonPressed();
    void on_Send_ButtonPressed();
    void on_viewDetails0_PushButton();
    void on_viewDetails1_PushButton();
    void on_viewDetails2_PushButton();
    void on_viewDetails3_PushButton();
    void on_viewDetails4_PushButton();
    void on_viewDetails5_PushButton();
    void on_TefreshTotalSendedLastWeekTimer();
};

#endif // WALLETSEND_H
