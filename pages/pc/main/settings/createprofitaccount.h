#ifndef CREATEPROFITACCOUNT_H
#define CREATEPROFITACCOUNT_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QTableView>
#include <QStandardItemModel>

#include "wallet/events.h"

class createprofitaccount : public QMainWindow
{
    Q_OBJECT
public:
    typedef enum {
        NONE = 0,
        RUN,
        CANCEL,
        OK,
    }runMode_e;
    typedef enum {
        RETURN_NONE = 0,
        RETURN_CANCEL,
        RETURN_OK
    }return_e;
    createprofitaccount();
    ~createprofitaccount();
    void init(QMdiArea *mdiArea = nullptr, int accCnt = -1);
    return_e run();
    runMode_e getState();
    void setState(runMode_e state, QString walletName = "");

    QMdiSubWindow *thisWindow = nullptr;
private:
    void refreshFonts();
    void refreshSize();
    void refreshLanguage();
    void refreshProfitingTable();

    QWidget *parent = nullptr;
    QMdiArea *thisMdiArea = nullptr;
    QWidget *thisWidget = nullptr;

    QLabel *titleName = nullptr;

    QLineEdit *accName = nullptr;
    QComboBox *accType = nullptr;
    QLineEdit *shareRatio = nullptr;
    QLineEdit *seats = nullptr;

    QPushButton *cancelButton = nullptr;
    QPushButton *okButton = nullptr;

    QTableView *profitingAccTableView = nullptr;
    QStandardItemModel *profitingAccItemModel = nullptr;


    runMode_e currentMode = NONE;
    runMode_e pastMode = NONE;
    return_e choose = RETURN_NONE;

    double pastScale = 0.0;
    QString pastLanguage = "";
    int accCnt = -1;
    rpc::network_e network = rpc::network_e::NETWORK_TESTNET;

private slots:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void on_Cancel_ButtonPressed();
    void on_Ok_ButtonPressed();
    void profitingAccItemClicked(QModelIndex);

};

#endif // CREATEPROFITACCOUNT_H
