#ifndef TRANSITIONSWINDOW_H
#define TRANSITIONSWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QLabel>
#include <QDateEdit>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QComboBox>
#include <QLineEdit>


#include "config.h"
#include "wallet/events.h"
#include "../wallet/walletshowdetails.h"

class transitionswindow : public QMainWindow {
    Q_OBJECT
public:
    typedef enum {
        STATE_NONE = 0,
        STATE_TRANSITIONS
    }state_e;

    transitionswindow();
    ~transitionswindow();
    void setVars(QMdiSubWindow *window = nullptr, QWidget *parent = nullptr);
    void run();
    void setState(state_e state);

    walletshowdetails *showDetailsWindow = nullptr;

private:
    void refreshTable();
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();
    /* Window */
    QWidget *parent = nullptr;
    QMdiSubWindow *window = nullptr;

    QMdiArea *mdiAreaTransitions = nullptr;
    QWidget *widgetTransitions = nullptr;
    QMdiSubWindow *windowTransitions = nullptr;

    QLabel *txDirectionLabel = nullptr;
    QComboBox *txDirectionComboBox = nullptr;

    QLabel *tokenLabel = nullptr;
    QComboBox *tokenComboBox = nullptr;

    QLabel *filterLabel = nullptr;
    QLineEdit *filterLineEdit = nullptr;

    QTableView *historyTableView = nullptr;
    QStandardItemModel *historyItemModel = nullptr;

    state_e currentState = STATE_NONE;
    state_e pastState = STATE_TRANSITIONS;
    double pastScale = 0.0;
    QString pastLanguage = "";

    QList<QList<QMap<QString, QString>>> cumulatedWallet;
    bool refreshTableSemaphore = false;

    int walletNameKeyListChanged = -1;
    int recentTxTransactionsModifyedCnt = -1;
    int selectedNameKeyIndex = -1;
    int assetsListModifyedCnt = -1;
    bool updateHistory = false;
    bool supressTableUpdate = false;
    int recentTransactionsCnt = -1;
    rpc::network_e network = rpc::network_e::NETWORK_TESTNET;
    QModelIndex oldHistoryAccItemIdx;
private slots:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void on_TxDirection_Changed(const QString &);
    void on_Token_Changed(const QString &);
    void on_Filter_Changed(const QString &);
    void historyAccItemClicked(QModelIndex);

};

#endif // TRANSITIONSWINDOW_H
