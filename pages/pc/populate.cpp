#include "populate.h"

#include "language/translate.h"
#include "crypto/signatures.h"
#include "wallet/rpc/wallethistory.h"
#include "pages/pc/textformating.h"
#include "wallet/events.h"
#include "configlyra.h"

bool variantLessThan(const QList<QStringList> &v1, const QList<QStringList> &v2)
{
    return v1.value(0) < v2.value(0);
}
struct QPairFirstComparer {
    template<typename T1, typename T2>
    bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const {
        return a.first < b.first;
    }
};

struct QPairFirstComparer1 {
    template<typename T1>
    /*bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const {
        return a.first < b.first;
    }*/
    bool operator()(const QList<T1> &v1, const QList<T1> &v2)
    {
        return v1.value(1) < v2.value(1);
    }
};

bool populate::refreshAll() {
    QList<QStringList> recentTransactionsList;
    int index = events::getSelectedNameKeyIndex();
    QStringList listId = events::getAccountNameList();
    if(index < 0 ) {
        if(listId.count() != 0) {
            index = 0;
        } else {
            return false;
        }
    }
    if(listId.count() == 0)
        return false;
    int transactionCount = 0;
    double balance = 0.0;
    QList<QPair<qint64, double>> allList;
    QList<QPair<QString, double>> tokList[listId.count()];
    for(int ind = 0; ind < listId.count(); ind++) {
        QString accountName = listId[ind];
        transactionCount = wallethistory::getCount(accountName);

        for(int cnt = 0; cnt < transactionCount; cnt++) {
            transaction_t transaction = wallethistory::getTransaction(accountName, cnt);
            qint64 time = wallethistory::timestampToQint64(transaction[2]["TimeStamp"]);
            balance = transaction[8]["LYR"].toDouble();
            allList.append(QPair<qint64, double>(time, transaction[7]["LYR"].toDouble()));
            if(cnt == 0)
                events::setCreationDateTime(time);
            QStringList tokenKeyList = transaction[8].keys();
            tokenKeyList.move(tokenKeyList.indexOf("LYR"), 0);
            if(cnt == transactionCount - 1) {
                foreach(QString token, tokenKeyList) {
                    tokList[ind].append(QPair<QString, double>(token, transaction[8][token].toDouble()));
                }
            }
            QDateTime date = QDateTime::fromMSecsSinceEpoch(time);
            int isReceived = transaction[1]["IsReceive"].toInt();
/* All transactions */
            QStringList recentTransactions;
            recentTransactions.append(accountName);
            recentTransactions.append(date.toString("yyyy-MM-dd hh:mm:ss"));
            if(isReceived) {
                recentTransactions.append("RECEIVED");
            } else {
                recentTransactions.append("SENT");
            }
            QList<QString>key = transaction[7].keys();
            QString value;
            QString tokenValue;
            foreach(QString tmp, key)  {
                if(transaction[7][tmp].toDouble() != 0.0) {
                    value = QString::number(isReceived ? transaction[7][tmp].toDouble() : 0.0 - transaction[7][tmp].toDouble());
                    tokenValue += "\n" + textformating::toValue(value, 2) + " " + tmp.replace("tether/", SYMBOL_FOR_TETHERED_TOKEN);
                }
            }
            if(tokenValue.length())
                tokenValue.remove(0, 1);
            else
                tokenValue = textformating::toValue(0, 2) + "LYR";
            recentTransactions.append(tokenValue);
            key = transaction[8].keys();
            tokenValue = "";
            foreach(QString tmp, key)  {
                if(transaction[8][tmp].toDouble() > 0.0) {
                    value = QString::number(transaction[8][tmp].toDouble());
                    tokenValue += "\n" + textformating::toValue(value, 2) + " " + tmp.replace("tether/", SYMBOL_FOR_TETHERED_TOKEN);
                }
            }
            if(tokenValue.length())
                tokenValue.remove(0, 1);
            else
                tokenValue = textformating::toValue(0, 2) + "LYR";
            recentTransactions.append(tokenValue);
            recentTransactionsList.append(recentTransactions);

        }
    }
    std::sort(recentTransactionsList.begin(), recentTransactionsList.end(), QPairFirstComparer1());
    std::reverse(recentTransactionsList.begin(), recentTransactionsList.end());
    std::sort(allList.begin(), allList.end(), QPairFirstComparer());
    double value = 0.0;
    for(int cnt = 0; cnt < allList.count(); cnt++) {
        value += allList[cnt].second;
        allList[cnt].second = value;
    }
    QList<QStringList> assets;
    QList<QPair<QString, double>> tokenAllValue;
    for(int cntAcc = 0; cntAcc < listId.count(); cntAcc++) {
        for(int tokenCnt = 0; tokenCnt < tokList[cntAcc].count(); tokenCnt++) {
            if(tokenAllValue.count()) {
                bool found = false;
                for(int cnt = 0;  cnt < tokenAllValue.count(); cnt++) {
                    if(!tokenAllValue[cnt].first.compare(tokList[cntAcc][tokenCnt].first)) {
                        tokenAllValue[cnt].second += tokList[cntAcc][tokenCnt].second;
                        found = true;
                    }
                }
                if(found == false) {
                    tokenAllValue.append(tokList[cntAcc][tokenCnt]);
                }
            } else {
                tokenAllValue.append(tokList[cntAcc][tokenCnt]);
            }
        }
    }
    QPair<QString, double> asset;
    int lyr = -1;
    foreach(asset, tokenAllValue) {
        QList<QString> assetsValue;
        assetsValue.append(asset.first.replace("tether/", SYMBOL_FOR_TETHERED_TOKEN));
        assetsValue.append("0.00");
        assetsValue.append(textformating::toValue(asset.second));
        assets.append(assetsValue);
        if(!asset.first.compare("LYR")) {
            lyr = assets.count() - 1;
        }
    }
    if(lyr != -1) {
        assets.move(lyr, 0);
    }
    events::setAssets(assets);


    QString accountName = listId[index];
    transactionCount = wallethistory::getCount(accountName);
    QList<QPair<qint64, double>> balanceList;
    QList<QPair<QString, double>> tokenList;
    QList<QPair<QString, QString>> rxTr;
    QList<QPair<QString, QString>> txTr;
    for(int cnt = 0; cnt < transactionCount; cnt++) {
        transaction_t transaction = wallethistory::getTransaction(accountName, cnt);
        qint64 time = wallethistory::timestampToQint64(transaction[2]["TimeStamp"]);
        balance = transaction[8]["LYR"].toDouble();
        balanceList.append(QPair<qint64, double>(time, transaction[8]["LYR"].toDouble()));
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
    events::setMyAccountValueAllChart(allList);
    events::setMyAccountValueChart(balanceList);
    events::setMyAccountBalanceChart(balanceList);
    events::setNumberOfTransactions(transactionCount);
    events::setBallance(balance);
    if(transactionCount == 0) {
        events::setCreationDateTime(0);
    }
    QList<QStringList> rxTransactions;
    QList<QStringList> txTransactions;
    if(transactionCount) {
        for (int cnt = transactionCount - 1; cnt >= 0; cnt--) {
            transaction_t transaction = wallethistory::getTransaction(accountName, cnt);
            int isReceived = transaction[1]["IsReceive"].toInt();
            qint64 time = wallethistory::timestampToQint64(transaction[2]["TimeStamp"]);
            QDateTime date = QDateTime::fromMSecsSinceEpoch(time);
/* All transactions */
            QStringList recentTransactions;
            QList<QString>key = transaction[7].keys();
            QString value;
            /*recentTransactions.append(date.toString("hh:mm:ss yyyy-MM-dd"));
            if(isReceived) {
                recentTransactions.append("RECEIVED");
            } else {
                recentTransactions.append("SENT");
            }
            if(key.count() > 1) {
                key.removeAll("LYR");
            }
            recentTransactions.append(key[0]);
            QString value = QString::number(isReceived ? transaction[7][key[0]].toDouble() : 0.0 - transaction[7][key[0]].toDouble());
            recentTransactions.append(textformating::toValue(value));
            value = transaction[8][key[0]];
            recentTransactions.append(textformating::toValue(value));

            recentTransactionsList.append(recentTransactions);*/

/* Sent Transactions */
/* Received Transactions */
            recentTransactions.clear();
            recentTransactions.append(date.toString("hh:mm:ss yyyy-MM-dd"));
            if(isReceived)
                recentTransactions.append(transaction[3]["SendAccountId"]);
            else
                recentTransactions.append(transaction[5]["RecvAccountId"]);
            QString tokenValue;
            foreach(QString tmp, key)  {
                value = QString::number(isReceived ? transaction[7][tmp].toDouble() : 0.0 - transaction[7][tmp].toDouble());
                tokenValue += "\n" + textformating::toValue(value, 2) + " " + tmp.replace("tether/", SYMBOL_FOR_TETHERED_TOKEN);
            }
            if(tokenValue.length()) {
                tokenValue.remove(0, 1);
            }

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
            /*if(cnt == transactionCount - 1) {
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
            }*/
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
