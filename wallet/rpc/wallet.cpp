
#include "wallet.h"

#include "wallet/events.h"
#include "wallet/rpc/walletbalance.h"
#include "crypto/signatures.h"
#include "wallet/rpc/wallethistory.h"
#include "pages/pc/populate.h"
#include "language/translate.h"
#include "wallet/tickedupdates.h"

void wallet::sync() {
    int index = events::getSelectedNameKeyIndex();
    QString accId = events::getAccountId(index);
    QStringList nameList = events::getAccountNameList();
    if(accId.length()) {
        events::setUnreceivedBallance("Please wait");
        bool newTransaction = false;
        if(walletErr_e::WALLET_ERR_OK != walletbalance::receive(index, &newTransaction)) {
            events::setUnreceivedBallance("ERROR!");
            return;
        }
        events::setUnreceivedBallance(newTransaction ? "Yes" : "No");
        bool unreceived = false;
        int height = 0;
        walletbalance::balance(accId, &height, &unreceived);
        events::setUnreceivedBallance(unreceived ? "Yes" : "No");
        if(newTransaction || height != wallethistory::getCount(nameList[index])) {
            wallethistory::updateAccount(nameList[index], accId);
            populate::refreshAll();
            events::setWalletHistoryChanged();
            events::setUpdateHistory();
            events::setTriggerNodeFetch();
        }
    }
}

void wallet::checkNewTransactions() {
    events::setUnreceivedBallance("Please wait");
    int index = events::getSelectedNameKeyIndex();
    bool newTransaction = false;
    int height = 0;
    QString accId = events::getAccountId(index);
    if(accId.length())
        walletbalance::balance(accId, &height, &newTransaction);
    events::setUnreceivedBallance(newTransaction ? "Yes" : "No");
}

void wallet::calculateLastWeek() {
    int index = events::getSelectedNameKeyIndex();
    QStringList nameList = events::getAccountNameList();
    if(nameList.count()) {
        QList<QList<QMap<QString, QString>>> wallet = wallethistory::getAccount(nameList[index]);
        qlonglong timeInterval =  QDateTime::currentDateTime().addDays(-7).toMSecsSinceEpoch();
        double amountSend = 0;
        double amountReceived = 0;
        for(int cnt = 0; cnt < wallet.count(); cnt++) {
            if(wallethistory::getTimeStamp(nameList[index], cnt).toDouble() > timeInterval) {
                if(!wallethistory::getIsReceive(nameList[index], cnt))
                    amountSend += wallethistory::getChanges(nameList[index], cnt, "LYR");
                else
                    amountReceived += wallethistory::getChanges(nameList[index], cnt, "LYR");
            }
        }
        events::setTotalLyraSendedLastWeek(amountSend);
        events::setTotalLyraReceivedLastWeek(amountReceived);
    }
}
