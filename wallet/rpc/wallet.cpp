
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
        events::setUnreceivedBallance("Please wait");
        bool newTransaction = false;
        walletbalance::receive(pair[index].second, &newTransaction);
        bool unreceived = false;
        int height = 0;
        walletbalance::balance(signatures::getAccountIdFromPrivateKey(pair[index].second), &height, &unreceived);
        events::setUnreceivedBallance(unreceived ? _tr("Yes") : _tr("No"));
        if(newTransaction || height != wallethistory::getCount(pair[index].first)) {
            wallethistory::updateWallet(pair[index].first, signatures::getAccountIdFromPrivateKey(pair[index].second));
            populate::refreshAll();
            events::setWalletHistoryChanged();
            events::setUpdateHistory();
        }
    }
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
