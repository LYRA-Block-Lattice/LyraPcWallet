#ifndef SWAPWINDOW_H
#define SWAPWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QTimer>
#include <QWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>

#include "wallet/events.h"
#include "wallet/rpc/pool.h"
#include "wallet/rpc/wallet.h"

class swapwindow : public QMainWindow
{
    Q_OBJECT
public:
    typedef enum {
        STATE_NONE = 0,
        STATE_SWAP
    }state_e;

    swapwindow();
    ~swapwindow();

    void setVars(QMdiSubWindow *window = nullptr, QWidget *parent = nullptr);
    void run();
    void setState(state_e state);
    QMdiSubWindow *window = nullptr;

    QMdiSubWindow *windowSwap = nullptr;
private:
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();
    void refreshTokenAmount();

    QWidget *parent = nullptr;

    QMdiArea *mdiAreaSwap = nullptr;
    QWidget *widgetSwap = nullptr;


    QLabel *fromLabel = nullptr;
    QLineEdit *fromValueLineEdit = nullptr;
    QLabel *fromAmountLabel = nullptr;
    QComboBox *fromValueComboBox = nullptr;

    QPushButton *reversePushButton = nullptr;

    QLabel *toLabel = nullptr;
    QLineEdit *toValueLineEdit = nullptr;
    QLabel *toAmountLabel = nullptr;
    QComboBox *toValueComboBox = nullptr;

    QLabel *totalLiquidateLabel = nullptr;
    QLabel *totalLiquidateValueLabel = nullptr;
    QLabel *estimatedPriceLabel = nullptr;
    QLabel *estimatedPriceValueLabel = nullptr;
    QLabel *youSellLabel = nullptr;
    QLabel *youSellValueLabel = nullptr;
    QLabel *youGetLabel = nullptr;
    QLabel *youGetValueLabel = nullptr;
    QLabel *priceimpactLabel = nullptr;
    QLabel *priceimpactValueLabel = nullptr;
    QLabel *poolFeeLabel = nullptr;
    QLabel *poolFeeValueLabel = nullptr;
    QLabel *liquidateFeeLabel = nullptr;
    QLabel *liquidateFeeValueLabel = nullptr;

    QPushButton *swapPushButton = nullptr;


    QTimer fetchPool;
    enum {
        VALUE_SOURCE_SELF = 0,
        VALUE_SOURCE_TOKEN0,
        VALUE_SOURCE_TOKEN1,
    }valueSource = VALUE_SOURCE_SELF;
    enum {
        LAST_EDITED_VAL_TOKEN0,
        LAST_EDITED_VAL_TOKEN1,
    }lastEditedVal = LAST_EDITED_VAL_TOKEN0;
    state_e currentState = STATE_NONE;
    state_e pastState = STATE_SWAP;
    double pastScale = 0.0;
    QString pastLanguage = "";
    int selectedNameKeyIndex = -1;
    int recentTransactionsCnt = -1;
    rpc::network_e network = rpc::network_e::NETWORK_TESTNET;
    double amountToken0 = 0.0;
    double amountToken1 = 0.0;

    pool::poolInfo_t poolInfo;
    pool::poolCalculate poolCalculate;
private slots:
    void on_fromValueLineEdit_textChanged(const QString &);
    void on_toValueLineEdit_textChanged(const QString &);
    void on_Ticker0_Changed(const QString &);
    void on_Ticker1_Changed(const QString &);
    void on_reverseTokens_ButtonPressed();
    void on_FetchPool();
    void on_swap_ButtonPressed();

};

#endif // SWAPWINDOW_H
