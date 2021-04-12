#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "rpcapi.h"
#include "rpc.h"

rpc *rpcConnection = nullptr;

rpcapi::rpcapi(rpc *rpcConn) {
    rpcConnection = rpcConn;
}

rpcapi::~rpcapi() {

}

connection_t rpcapi::getConnection() {
    return rpcConnection;
}

QList<QList<QMap<QString, QString>>> rpcapi::history(qint64 timeStart, qint64 timeEnd, int count) {
    QList<QList<QMap<QString, QString>>> list;
    if(rpcConnection == nullptr) {
        return list;
    }
    //QDateTime lm = QDateTime::currentDateTimeUtc();
    int id = 0;
    QString response = rpcConnection->sendMessage(&id, "History",
                                                  QStringList({"LF6LpgcrANQWrErPcLREAbKzJg9DLeuXXa45cz5hKsUng7aJ2zCrAgHbtkSXv5dXiEfUB8ypN8i3daUkmiJwcX8cbXSv5U",
                                                  QString::number(timeStart),
                                                  QString::number(timeEnd),
                                                  QString::number(count)}));
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
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
        QJsonObject changesObj;
        QMap<QString, QString> changes;
        QJsonObject balancesObj;
        QMap<QString, QString> balances;
        QJsonObject obj = value.toObject();

        height.insert("Height", QString::number(obj["Height"].toInt()));
        isReceive.insert("IsReceive", QString::number(obj["IsReceive"].toBool()));
        timeStamp.insert("TimeStamp", obj["TimeStamp"].toString());
        sendAccountId.insert("SendAccountId", obj["SendAccountId"].toString());
        sendHash.insert("SendHash", obj["SendHash"].toString());
        recvAccountId.insert("RecvAccountId", obj["RecvAccountId"].toString());
        recvHash.insert("RecvHash", obj["RecvHash"].toString());

        changesObj = obj["Changes"].toObject();
        QStringList changesKeyList = changesObj.keys();
        foreach(QString key,  changesKeyList) {
            changes.insert(key, QString::number(changesObj[key].toVariant().toULongLong()));
        }

        balancesObj = obj["Balances"].toObject();
        QStringList balancesKeyList = balancesObj.keys();
        foreach(QString key,  balancesKeyList) {
            balances.insert(key , QString::number(balancesObj[key].toVariant().toULongLong()));
        }
        list[list.count() - 1].append(height);
        list[list.count() - 1].append(isReceive);
        list[list.count() - 1].append(timeStamp);
        list[list.count() - 1].append(sendAccountId);
        list[list.count() - 1].append(sendHash);
        list[list.count() - 1].append(recvAccountId);
        list[list.count() - 1].append(recvHash);
        list[list.count() - 1].append(changes);
        list[list.count() - 1].append(balances);
    }
    return list;
}

QMap<QString, double> rpcapi::balance(bool *unreceived) {
    if(rpcConnection == nullptr) {
        return QMap<QString, double>();
    }
    int id = 0;
    QString response = rpcConnection->sendMessage(&id, "Balance",
                                                  QStringList({"LF6LpgcrANQWrErPcLREAbKzJg9DLeuXXa45cz5hKsUng7aJ2zCrAgHbtkSXv5dXiEfUB8ypN8i3daUkmiJwcX8cbXSv5U"}));
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonObject jsonResult = jsonObject["result"].toObject();
    QJsonObject jsonBalance = jsonResult["balance"].toObject();
    *unreceived = jsonObject["unreceived"].toBool();
    QMap<QString, double> balance;
    QStringList balanceKeyList = jsonBalance.keys();
    foreach(QString key,  balanceKeyList) {
        balance.insert(key , jsonBalance[key].toDouble());
    }
    return balance;
}
