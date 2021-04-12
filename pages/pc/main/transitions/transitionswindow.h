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

class transitionswindow : public QMainWindow
{
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

    walletshowdetails *showDetailsWindow;

private:
    void refreshTable();
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();
    /* Window */
    QWidget *parent;
    QMdiSubWindow *window;
    //QMdiArea *mdiArea;
    QMdiArea *mdiAreaTransitions;
    QWidget *widgetTransitions;
    QMdiSubWindow *windowTransitions;

    QLabel *txDirectionLabel;
    QComboBox *txDirectionComboBox;

    QLabel *tokenLabel;
    QComboBox *tokenComboBox;

    QLabel *filterLabel;
    QLineEdit *filterLineEdit;

    QTableView *historyTableView;
    QStandardItemModel *historyItemModel;

    state_e currentState = STATE_NONE;
    state_e pastState = STATE_TRANSITIONS;
    double pastScale = 0.0;
    QString pastLanguage = "en";

    QList<QList<QMap<QString, QString>>> cumulatedWallet;
    bool refreshTableSemaphore = false;

    int walletNameKeyListChanged = -1;
    int recentTxTransactionsModifyedCnt = -1;
    int selectedNameKeyIndex = -1;
    int assetsListModifyedCnt = -1;
    bool updateHistory = false;
    rpc::network_e network = rpc::network_e::NETWORK_TESTNET;
private slots:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void on_TxDirection_Changed(const QString &);
    void on_Token_Changed(const QString &);
    void on_Filter_Changed(const QString &);

};

#endif // TRANSITIONSWINDOW_H
