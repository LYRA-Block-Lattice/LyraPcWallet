#include "populate.h"

#include "language/translate.h"
#include "crypto/signatures.h"
#include "wallet/rpc/wallethistory.h"
#include "pages/pc/textformating.h"
#include "wallet/events.h"

bool populate::refreshAll() {
    int index = events::getSelectedNameKeyIndex();
    QList<QPair<QString, QString>> pair = events::getWalletNameKeyList();
    if(index < 0 ) {
        if(pair.count() != 0) {
            index = 0;
        } else {
            return false;
        }
    }
    if(pair.count() == 0)
        return false;
    QString walletName = pair[index].first;
    int transactionCount = wallethistory::getCount(walletName);
    QList<QPair<qint64, double>> allList;
    double balance = 0.0;
    QList<QPair<QString, double>> tokenList;
    QList<QPair<QString, QString>> rxTr;
    QList<QPair<QString, QString>> txTr;
    for(int cnt = 0; cnt < transactionCount; cnt++) {
        transaction_t transaction = wallethistory::getTransaction(walletName, cnt);
        qint64 time = wallethistory::timestampToQint64(transaction[2]["TimeStamp"]);
        balance = transaction[8]["LYR"].toDouble();
        allList.append(QPair<qint64, double>(time, transaction[8]["LYR"].toDouble()));
        if(cnt == 0)
            events::setCreationDateTime(time);
        QStringList tokenKeyList = transaction[8].keys();
        tokenKeyList.move(tokenKeyList.indexOf("LYR"), 0);
        if(cnt == transactionCount - 1) {
            foreach(QString token, tokenKeyList) {
                tokenList.append(QPair<QString, double>(token, transaction[8][token].toDouble()));
            }
        }
    }

    events::setTokenList(tokenList);
    events::setMyWalletValueAllChart(allList);
    events::setMyWalletValueChart(allList);
    events::setMyWalletBalanceChart(allList);
    events::setNumberOfTransactions(transactionCount);
    events::setBallance(balance);
    if(transactionCount == 0) {
        events::setCreationDateTime(0);
    }
    QList<QStringList> recentTransactionsList;
    QList<QStringList> assets;
    QList<QStringList> rxTransactions;
    QList<QStringList> txTransactions;
    if(transactionCount) {
        for (int cnt = transactionCount - 1; cnt >= 0; cnt--) {
            transaction_t transaction = wallethistory::getTransaction(walletName, cnt);
            int isReceived = transaction[1]["IsReceive"].toInt();
            qint64 time = wallethistory::timestampToQint64(transaction[2]["TimeStamp"]);
            QDateTime date = QDateTime::fromMSecsSinceEpoch(time);
/* All transactions */
            QStringList recentTransactions;
            recentTransactions.append(date.toString("hh:mm:ss yyyy-MM-dd"));
            if(isReceived) {
                recentTransactions.append("RECEIVE");
            } else {
                recentTransactions.append("SENDED");
            }
            QList<QString>key = transaction[7].keys();
            if(key.count() > 1) {
                key.removeAll("LYR");
            }
            recentTransactions.append(key[0]);
            QString value = QString::number(isReceived ? transaction[7][key[0]].toDouble() : 0.0 - transaction[7][key[0]].toDouble());
            recentTransactions.append(textformating::toValue(value));
            value = transaction[8][key[0]];
            recentTransactions.append(textformating::toValue(value));

            recentTransactionsList.append(recentTransactions);

/* Sended Transactions */
/* Received Transactions */
            recentTransactions.clear();
            recentTransactions.append(date.toString("hh:mm:ss yyyy-MM-dd"));
            if(isReceived)
                recentTransactions.append(transaction[3]["SendAccountId"]);
            else
                recentTransactions.append(transaction[5]["RecvAccountId"]);
            value = QString::number(isReceived ? transaction[7][key[0]].toDouble() : 0.0 - transaction[7][key[0]].toDouble());
            QString tokenValue;
            tokenValue = textformating::toValue(value) + " " + key[0];
            recentTransactions.append(tokenValue);
            recentTransactions.append(_tr("View details"));
            recentTransactions.append(_tr("Completed"));
            if(transaction.count() >= 10) {
                recentTransactions.append(transaction[9].value("note"));
            } else {
                recentTransactions.append("-");
            }
            recentTransactions.append(transaction[0]["Height"]);
            if(isReceived) {
                rxTransactions.append(recentTransactions);
            } else {
                txTransactions.append(recentTransactions);
            }
            if(isReceived) {
                rxTr.append(QPair<QString, QString>(textformating::toDate(time), tokenValue));
            } else {
                txTr.append(QPair<QString, QString>(textformating::toDate(time), tokenValue));
            }
/* Assets */
            if(cnt == transactionCount - 1) {
                QString asset;
                QStringList assetList = transaction[8].keys();
                foreach(QString assName, assetList) {
                    QList<QString> assetsValue;
                    asset = transaction[8][assName];
                    assetsValue.append(assName);
                    assetsValue.append("0.00");
                    assetsValue.append(textformating::toValue(asset));
                    assets.append(assetsValue);
                }
                events::setAssets(assets);
            }
        }
        events::setLastReceivedTransactionsList(rxTr);
        events::setLastSendedTransactionsList(txTr);
        events::setRecentTransactions(recentTransactionsList);
        events::setRecentRxTransactions(rxTransactions);
        events::setRecentTxTransactions(txTransactions);
    } else {
        events::setLastReceivedTransactionsList(rxTr);
        events::setLastSendedTransactionsList(txTr);
        events::setRecentTransactions(recentTransactionsList);
        events::setAssets(assets);
        events::setRecentRxTransactions(rxTransactions);
        events::setRecentTxTransactions(txTransactions);
    }
    return true;
}
