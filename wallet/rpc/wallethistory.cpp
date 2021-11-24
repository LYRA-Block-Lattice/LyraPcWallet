#include <QDateTime>

#include "wallethistory.h"
#include "network/rpcapi.h"
#include "wallet/events.h"
#include "crypto/signatures.h"

QMap<QString, QList<QList<QMap<QString, QString>>>> walletHistory[2];

QList<QList<QMap<QString, QString>>> wallethistory::parseData(QString data) {
    qDebug() << "WALLETHISTORY 1 Start parse data";
    QList<QList<QMap<QString, QString>>> list;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    //QString jsonrpc = jsonResponse["jsonrpc"].toString();
    //QString id = jsonResponse["id"].toString();
    QJsonArray jsonArray = jsonObject["result"].toArray();
    foreach (const QJsonValue & value, jsonArray) {
        list.append(QList<QMap<QString, QString>>());

        QMap<QString, QString> height;
        QMap<QString, QString> isReceive;
        QMap<QString, QString> timeStamp;
        QMap<QString, QString> sendAccountId;
        QMap<QString, QString> sendHash;
        QMap<QString, QString> recvAccountId;
        QMap<QString, QString> recvHash;
        QJsonObject eventsObj;
        QMap<QString, QString> events;
        QJsonObject balancesObj;
        QMap<QString, QString> balances;
        QMap<QString, QString> note;
        QJsonObject obj = value.toObject();

        height.insert("Height", QString::number(obj["Height"].toInt()));
        isReceive.insert("IsReceive", QString::number(obj["IsReceive"].toBool()));
        timeStamp.insert("TimeStamp", obj["TimeStamp"].toVariant().toString());
        sendAccountId.insert("SendAccountId", obj["SendAccountId"].toString());
        sendHash.insert("SendHash", obj["SendHash"].toString());
        recvAccountId.insert("RecvAccountId", obj["RecvAccountId"].toString());
        recvHash.insert("RecvHash", obj["RecvHash"].toString());
        note.insert("note", obj["note"].toString());

        eventsObj = obj["Changes"].toObject();
        QStringList eventsKeyList = eventsObj.keys();
        foreach(QString key,  eventsKeyList) {
            events.insert(key, eventsObj[key].toString());
        }

        balancesObj = obj["Balances"].toObject();
        QStringList balancesKeyList = balancesObj.keys();
        foreach(QString key,  balancesKeyList) {
            balances.insert(key , balancesObj[key].toString());
        }
        list[list.count() - 1].append(height);
        list[list.count() - 1].append(isReceive);
        list[list.count() - 1].append(timeStamp);
        list[list.count() - 1].append(sendAccountId);
        list[list.count() - 1].append(sendHash);
        list[list.count() - 1].append(recvAccountId);
        list[list.count() - 1].append(recvHash);
        list[list.count() - 1].append(events);
        list[list.count() - 1].append(balances);
        list[list.count() - 1].append(note);
    }
    qDebug() << "WALLETHISTORY 2: End parse data";
    return list;
}

QList<QList<QMap<QString, QString>>> wallethistory::parseData(QJsonArray jsonArray) {
    qDebug() << "WALLETHISTORY 3: Start parse data";
    QList<QList<QMap<QString, QString>>> list;
    foreach (const QJsonValue & value, jsonArray) {
        list.append(QList<QMap<QString, QString>>());

        QMap<QString, QString> height;
        QMap<QString, QString> isReceive;
        QMap<QString, QString> timeStamp;
        QMap<QString, QString> sendAccountId;
        QMap<QString, QString> sendHash;
        QMap<QString, QString> recvAccountId;
        QMap<QString, QString> recvHash;
        QJsonObject eventsObj;
        QMap<QString, QString> events;
        QJsonObject balancesObj;
        QMap<QString, QString> balances;
        QMap<QString, QString> note;
        QJsonObject obj = value.toObject();

        height.insert("Height", QString::number(obj["Height"].toInt()));
        isReceive.insert("IsReceive", QString::number(obj["IsReceive"].toBool()));
        timeStamp.insert("TimeStamp", obj["TimeStamp"].toVariant().toString());
        sendAccountId.insert("SendAccountId", obj["SendAccountId"].toString());
        sendHash.insert("SendHash", obj["SendHash"].toString());
        recvAccountId.insert("RecvAccountId", obj["RecvAccountId"].toString());
        recvHash.insert("RecvHash", obj["RecvHash"].toString());
        note.insert("note", obj["note"].toString());

        eventsObj = obj["Changes"].toObject();
        QStringList eventsKeyList = eventsObj.keys();
        foreach(QString key,  eventsKeyList) {
            events.insert(key, eventsObj[key].toString());
        }

        balancesObj = obj["Balances"].toObject();
        QStringList balancesKeyList = balancesObj.keys();
        foreach(QString key,  balancesKeyList) {
            balances.insert(key , balancesObj[key].toString());
        }
        list[list.count() - 1].append(height);
        list[list.count() - 1].append(isReceive);
        list[list.count() - 1].append(timeStamp);
        list[list.count() - 1].append(sendAccountId);
        list[list.count() - 1].append(sendHash);
        list[list.count() - 1].append(recvAccountId);
        list[list.count() - 1].append(recvHash);
        list[list.count() - 1].append(events);
        list[list.count() - 1].append(balances);
        list[list.count() - 1].append(note);
    }
    qDebug() << "WALLETHISTORY 4: End parse data";
    return list;
}

QJsonArray wallethistory::getAccountHistoryJson(QString name, int network) {
    qDebug() << "WALLETHISTORY 5: Start get wallet history";
    QList<QList<QMap<QString, QString>>> wallet = walletHistory[network == -1 ? events::getNetwork() : network].find(name).value();//getWallet(name);
    QList<QMap<QString, QString>> transactionList;
    QJsonArray transactionsObject;
    foreach (transactionList, wallet) {
        QJsonObject transactionObject;
        transactionObject.insert("Height", transactionList[0].value("Height").toInt());
        transactionObject.insert("IsReceive", (bool)transactionList[1].value("IsReceive").toInt() == 1);
        transactionObject.insert("TimeStamp", transactionList[2].value("TimeStamp").toLongLong());
        transactionObject.insert("SendAccountId", transactionList[3].value("SendAccountId"));
        transactionObject.insert("SendHash", transactionList[4].value("SendHash"));
        transactionObject.insert("RecvAccountId", transactionList[5].value("RecvAccountId"));
        transactionObject.insert("RecvHash", transactionList[6].value("RecvHash"));
        QJsonObject changeObject;
        QStringList tmp = transactionList[7].keys();
        foreach(QString key,  tmp) {
            changeObject.insert(key, transactionList[7].value(key));
        }
        transactionObject.insert("Changes", changeObject);
        QJsonObject balanceObject;
        tmp = transactionList[8].keys();
        foreach(QString key,  tmp) {
            balanceObject.insert(key, transactionList[8].value(key));
        }
        transactionObject.insert("Balances", balanceObject);
        if(transactionList.count() >= 10) {
            transactionObject.insert("note", transactionList[9].value("note"));
        }
        transactionsObject.append(transactionObject);
    }
    qDebug() << "WALLETHISTORY 6: End get wallet history";
    return transactionsObject;
}

bool wallethistory::addAccount(QString name, int network) {
    if(walletHistory[network == -1 ? events::getNetwork() : network].contains(name))
        return false;
    walletHistory[network == -1 ? events::getNetwork() : network].insert(name, parseData(""));
    return true;
}

bool wallethistory::addAccount(QString name, QString accountId, int network) {
    if(walletHistory[network == -1 ? events::getNetwork() : network].contains(name))
        return false;
    QDateTime lm = QDateTime::currentDateTimeUtc();
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "History",
                                                  QStringList({accountId,
                                                  QString::number(0),
                                                  QString::number(lm.toMSecsSinceEpoch()),
                                                  QString::number(0)}));
    if(response.length() == 0)
        return false;
    walletHistory[network == -1 ? events::getNetwork() : network].insert(name, parseData(response));
    return true;
}

bool wallethistory::addAccount(QString name, QJsonArray jsonArray, int network) {
    if(walletHistory[network == -1 ? events::getNetwork() : network].contains(name))
        return false;
    walletHistory[network == -1 ? events::getNetwork() : network].insert(name, parseData(jsonArray));
    return true;
}

bool wallethistory::removeAccount(QString name){
    if(!walletHistory[0].contains(name))
        return false;
    walletHistory[0].remove(name);
    if(!walletHistory[1].contains(name))
        return false;
    walletHistory[1].remove(name);
    return true;
}

int wallethistory::getAccountsCount() {
    return walletHistory[events::getNetwork()].count();
}

QList<QList<QMap<QString, QString>>> wallethistory::getAccount(QString name) {
    QMap<QString, QList<QList<QMap<QString, QString>>>>::iterator i = walletHistory[events::getNetwork()].find(name);
    return i.value();
}

QMap<QString, QList<QList<QMap<QString, QString>>>> wallethistory::getAccounts(int network) {
    return walletHistory[network == -1 ? events::getNetwork() : network];
}

bool wallethistory::setNote(QString accountName, int transaction, QString note) {
    QList<QList<QMap<QString, QString>>> srcAcc = getAccount(accountName);
    if(srcAcc[transaction].count() >= 10)
        srcAcc[transaction].replace(9, QMap<QString, QString>({{"note", note}}));
    else
        srcAcc[transaction].append(QMap<QString, QString>({{"note", note}}));
    walletHistory[events::getNetwork()].remove(accountName);
    walletHistory[events::getNetwork()].insert(accountName, srcAcc);
    return true;
}

bool wallethistory::updateAccount(QString name, QString accountId) {
    qDebug() << "WALLETHISTORY 7: Start update wallet history";
    int network = events::getNetwork();
    if(!walletHistory[network].contains(name)) {
        qDebug() << "WALLETHISTORY 8: Update wallet history";
        return false;
    }
    QDateTime lm = QDateTime::currentDateTimeUtc();
    connection_t connection = rpcapi::getConnection();
    QString response;
    int id = 0;
    response = connection->sendMessage(&id, "History",
                                                  QStringList({accountId,
                                                  QString::number(0),
                                                  QString::number(lm.toMSecsSinceEpoch()),
                                                  QString::number(0)}));
    if(response.length() == 0) {
        qDebug() << "WALLETHISTORY 9: Update wallet history";
        return false;
    }
    QList<QList<QMap<QString, QString>>> srcAcc = getAccount(name);
    QList<QList<QMap<QString, QString>>> destAcc = parseData(response);
    for(int cnt = 0; cnt < srcAcc.count(); cnt++) {
        if(cnt < destAcc.count() && srcAcc[cnt].count() >= 10) {
            if(destAcc[cnt].count() >= 10)
                destAcc[cnt].replace(9, srcAcc[cnt][9]);
            else
                destAcc[cnt].append(srcAcc[cnt][9]);
        }
    }
    walletHistory[network].remove(name);
    walletHistory[network].insert(name, destAcc);
    if(srcAcc.count() != destAcc.count()) {
        events::setWalletHistoryChanged();
    }
    qDebug() << "WALLETHISTORY 10: End update wallet history";
    return true;
}

bool wallethistory::updateAccountName(QString oldName, QString newName) {
    QList<QList<QMap<QString, QString>>> tmp = walletHistory[0][oldName];
    if(walletHistory[0].contains(oldName)) {
        tmp = walletHistory[0][oldName];
        walletHistory[0].remove(oldName);
        walletHistory[0].insert(newName, tmp);
    }
    if(walletHistory[1].contains(oldName)) {
        tmp = walletHistory[1][oldName];
        walletHistory[1].remove(oldName);
        walletHistory[1].insert(newName, tmp);
    }
    return true;
}

bool wallethistory::updateAccounts() {
    qDebug() << "WALLETHISTORY 11: Start update wallets";
    QPair<QString, QString> pair;
    QList<QPair<QString, QString>> list = events::getAccountNameIdList();
    foreach(pair, list) {
        wallethistory::updateAccount(pair.first, pair.second);
    }
    qDebug() << "WALLETHISTORY 12: End update wallets";
    return true;
}

int wallethistory::getCount(QString name) {
    QMap<QString, QList<QList<QMap<QString, QString>>>>::iterator i = walletHistory[events::getNetwork()].find(name);
    if(i == walletHistory[events::getNetwork()].end())
        return 0;
    return i.value().count();
}

int wallethistory::getHeight(QString name, int transaction) {
    QMap<QString, QList<QList<QMap<QString, QString>>>>::iterator i = walletHistory[events::getNetwork()].find(name);
    if(i == walletHistory[events::getNetwork()].end() || transaction >= i.value().count())
        return 0;
    QMap<QString, QString> pair = i.value().at(transaction).at(HEIGHT);
    if(!pair.contains("Height"))
        return 0;
    return pair.first().toInt();
}

bool wallethistory::getIsReceive(QString name, int transaction) {
    QMap<QString, QList<QList<QMap<QString, QString>>>>::iterator i = walletHistory[events::getNetwork()].find(name);
    if(i == walletHistory[events::getNetwork()].end() || transaction >= i.value().count())
        return 0;
    QMap<QString, QString> pair = i.value().at(transaction).at(IS_RECEIVE);
    if(!pair.contains("IsReceive"))
        return 0;
    return pair.first().toInt() == 1 ? true : false;
}

QString wallethistory::getTimeStamp(QString name, int transaction) {
    QMap<QString, QList<QList<QMap<QString, QString>>>>::iterator i = walletHistory[events::getNetwork()].find(name);
    if(i == walletHistory[events::getNetwork()].end() || transaction >= i.value().count())
        return 0;
    QMap<QString, QString> pair = i.value().at(transaction).at(TIME_STAMP);
    if(!pair.contains("TimeStamp"))
        return 0;
    return pair.first();
}

QString wallethistory::getSendAccId(QString name, int transaction) {
    QMap<QString, QList<QList<QMap<QString, QString>>>>::iterator i = walletHistory[events::getNetwork()].find(name);
    if(i == walletHistory[events::getNetwork()].end() || transaction >= i.value().count())
        return 0;
    QMap<QString, QString> pair = i.value().at(transaction).at(SEND_ACC_ID);
    if(!pair.contains("SendAccountId"))
        return 0;
    return pair.first();
}

QString wallethistory::getSendHash(QString name, int transaction) {
    QMap<QString, QList<QList<QMap<QString, QString>>>>::iterator i = walletHistory[events::getNetwork()].find(name);
    if(i == walletHistory[events::getNetwork()].end() || transaction >= i.value().count())
        return 0;
    QMap<QString, QString> pair = i.value().at(transaction).at(SEND_HASH);
    if(!pair.contains("SendHash"))
        return 0;
    return pair.first();
}

QString wallethistory::getRecvAccId(QString name, int transaction) {
    QMap<QString, QList<QList<QMap<QString, QString>>>>::iterator i = walletHistory[events::getNetwork()].find(name);
    if(i == walletHistory[events::getNetwork()].end() || transaction >= i.value().count())
        return 0;
    QMap<QString, QString> pair = i.value().at(transaction).at(RECV_ACC_ID);
    if(!pair.contains("RecvAccountId"))
        return 0;
    return pair.first();
}

QString wallethistory::getRecvHash(QString name, int transaction) {
    QMap<QString, QList<QList<QMap<QString, QString>>>>::iterator i = walletHistory[events::getNetwork()].find(name);
    if(i == walletHistory[events::getNetwork()].end() || transaction >= i.value().count())
        return 0;
    QMap<QString, QString> pair = i.value().at(transaction).at(RECV_HASH);
    if(!pair.contains("RecvHash"))
        return 0;
    return pair.first();
}

QStringList wallethistory::getChangesTokens(QString name, int transaction) {
    QMap<QString, QList<QList<QMap<QString, QString>>>>::iterator i = walletHistory[events::getNetwork()].find(name);
    if(i == walletHistory[events::getNetwork()].end() || transaction >= i.value().count())
        return QStringList();
    QMap<QString, QString> pair = i.value().at(transaction).at(CHANGES);
    return pair.keys();
}

double wallethistory::getChanges(QString name, int transaction, QString token) {
    QMap<QString, QList<QList<QMap<QString, QString>>>>::iterator i = walletHistory[events::getNetwork()].find(name);
    if(i == walletHistory[events::getNetwork()].end() || transaction >= i.value().count())
        return 0;
    QMap<QString, QString> pair = i.value().at(transaction).at(CHANGES);
    if(!pair.contains(token))
        return 0;
    QMap<QString, QString>::iterator val = pair.find(token);
    if(val == pair.end())
        return 0;
    QString value(val.value());
    if(value.length() > 8) {
        value.insert(8, '.');
    } else if(value.length() <= 8) {
        value = value.rightJustified(8 - value.length(), '0');
    }
    return value.toDouble();
}

QStringList wallethistory::getBalancesTokens(QString name, int transaction) {
    QMap<QString, QList<QList<QMap<QString, QString>>>>::iterator i = walletHistory[events::getNetwork()].find(name);
    if(i == walletHistory[events::getNetwork()].end() || transaction >= i.value().count())
        return QStringList();
    QMap<QString, QString> pair = i.value().at(transaction).at(BALANCES);
    return pair.keys();
}

double wallethistory::getBalance(QString name, int transaction, QString token) {
    QMap<QString, QList<QList<QMap<QString, QString>>>>::iterator i = walletHistory[events::getNetwork()].find(name);
    if(i == walletHistory[events::getNetwork()].end() || transaction >= i.value().count())
        return 0;
    QMap<QString, QString> pair = i.value().at(transaction).at(BALANCES);
    if(!pair.contains(token))
        return 0;
    QMap<QString, QString>::iterator val = pair.find(token);
    if(val == pair.end())
        return 0;
    QString value(val.value());
    if(value.length() > 8) {
        value.insert(8, '.');
    } else if(value.length() <= 8) {
        value = value.rightJustified(8 - value.length(), '0');
    }
    return value.toDouble();
}

transaction_t wallethistory::getTransaction(QString name, int transaction) {
    QMap<QString, QList<QList<QMap<QString, QString>>>>::iterator i = walletHistory[events::getNetwork()].find(name);
    if(i == walletHistory[events::getNetwork()].end() || transaction >= i.value().count())
        return QList<QMap<QString, QString>>();
    QList<QMap<QString, QString>> tr = i.value().at(transaction);
    return tr;
}

qint64 wallethistory::timestampToQint64(QString timestamp) {
    QDateTime time = QDateTime();
    time.setMSecsSinceEpoch(timestamp.toLongLong());
    return time.toMSecsSinceEpoch();
}
