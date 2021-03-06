#ifndef WALLETHISTORY_H
#define WALLETHISTORY_H

#include <QMap>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "wallet.h"

class wallethistory
{
public:
    typedef enum {
        HEIGHT = 0,
        IS_RECEIVE,
        TIME_STAMP,
        SEND_ACC_ID,
        SEND_HASH,
        RECV_ACC_ID,
        RECV_HASH,
        CHANGES,
        BALANCES
    }variable_e;

    static bool addAccount(QString name, int network = -1);
    static bool addAccount(QString name, QString accountId, int network = -1);
    static bool addAccount(QString name, QJsonArray jsonArray, int network = -1);
    static QJsonArray getAccountHistoryJson(QString name, int network = -1);
    static QJsonObject getAccountJson();
    static bool removeAccount(QString name);
    static bool updateAccount(QString name, QString accountId);
    static bool updateAccountName(QString oldName, QString newName);
    static bool updateAccounts();
    static int getAccountsCount();
    static QList<QList<QMap<QString, QString>>> getAccount(QString name);
    static QMap<QString, QList<QList<QMap<QString, QString>>>> getAccounts(int network = -1);
    static int getCount(QString name);
    static bool setNote(QString accountName, int transaction, QString note);

    static int getHeight(QString name, int transaction);
    static bool getIsReceive(QString name, int transaction);
    static QString getTimeStamp(QString name, int transaction);
    static QString getSendAccId(QString name, int transaction);
    static QString getSendHash(QString name, int transaction);
    static QString getRecvAccId(QString name, int transaction);
    static QString getRecvHash(QString name, int transaction);
    static QStringList getChangesTokens(QString name, int transaction);
    static double getChanges(QString name, int transaction, QString token);
    static QStringList getBalancesTokens(QString name, int transaction);
    static double getBalance(QString name, int transaction, QString token);
    static transaction_t getTransaction(QString name, int transaction);
    static qint64 timestampToQint64(QString timestamp);

private:
    static QList<QList<QMap<QString, QString>>> parseData(QString data);
    static QList<QList<QMap<QString, QString>>> parseData(QJsonArray jsonArray);
};

#endif // WALLETHISTORY_H
