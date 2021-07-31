
#include "wallet.h"

#include "wallet/events.h"
#include "wallet/rpc/walletbalance.h"
#include "crypto/signatures.h"
#include "wallet/rpc/wallethistory.h"
#include "pages/pc/populate.h"
#include "language/translate.h"

void wallet::sync() {
    int index = events::getSelectedNameKeyIndex();
    QList<QPair<QString, QString>> pair = events::getWalletNameKeyList();
    if(pair.count()) {
        events::setUnreceivedBallance(_tr("Please wait"));
        bool newTransaction = false;
        walletbalance::receive(pair[index].second, &newTransaction);
        events::setUnreceivedBallance(newTransaction ? "Yes" : "No");
        bool unreceived = false;
        int height = 0;
        walletbalance::balance(signatures::getAccountIdFromPrivateKey(pair[index].second), &height, &unreceived);
        events::setUnreceivedBallance(unreceived ? "Yes" : "No");
        if(newTransaction || height != wallethistory::getCount(pair[index].first)) {
            wallethistory::updateWallet(pair[index].first, signatures::getAccountIdFromPrivateKey(pair[index].second));
            populate::refreshAll();
            events::setWalletHistoryChanged();
            events::setUpdateHistory();
        }
    }
}

void wallet::checkNewTransactions() {
    events::setUnreceivedBallance(_tr("Please wait"));
    int index = events::getSelectedNameKeyIndex();
    QList<QPair<QString, QString>> pair = events::getWalletNameKeyList();
    bool newTransaction = false;
    int height = 0;
    walletbalance::balance(signatures::getAccountIdFromPrivateKey(pair[index].second), &height, &newTransaction);
    events::setUnreceivedBallance(newTransaction ? "Yes" : "No");
}

void wallet::calculateLastWeek() {
    int index = events::getSelectedNameKeyIndex();
    QList<QPair<QString, QString>> pair = events::getWalletNameKeyList();
    if(pair.count() != 0) {
        QList<QList<QMap<QString, QString>>> wallet = wallethistory::getWallet(pair[index].first);
        qlonglong timeInterval =  QDateTime::currentDateTime().addDays(-7).toMSecsSinceEpoch();
        double amountSend = 0;
        double amountReceived = 0;
        for(int cnt = 0; cnt < wallet.count(); cnt++) {
            if(wallethistory::getTimeStamp(pair[index].first, cnt).toDouble() > timeInterval) {
                if(!wallethistory::getIsReceive(pair[index].first, cnt)) {
                    amountSend += wallethistory::getChanges(pair[index].first, cnt, "LYR");
                } else {
                    amountReceived += wallethistory::getChanges(pair[index].first, cnt, "LYR");
                }
            }
        }
        events::setTotalLyraSendedLastWeek(amountSend);
        events::setTotalLyraReceivedLastWeek(amountReceived);
    }
}
