#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QApplication>

#include "walletbalance.h"
#include "network/rpcapi.h"
#include "crypto/signatures.h"

ballance_t walletbalance::balance(QString accountId, int *height, bool *unreceived) {
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "Balance",
                                                  QStringList({accountId}));
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonObject jsonResult = jsonObject["result"].toObject();
    QJsonObject jsonBalance = jsonResult["balance"].toObject();
    *unreceived = jsonResult["unreceived"].toBool();
    *height = jsonResult["height"].toInt();
    QMap<QString, double> balance;
    QStringList balanceKeyList = jsonBalance.keys();
    foreach(QString key,  balanceKeyList) {
        balance.insert(key , jsonBalance[key].toDouble());
    }
    return balance;
}

walletErr_e walletbalance::monitor(QString accountId, bool */*unreceived*/) {
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "Monitor",
                                                  QStringList({accountId}));
    return walletErr_e::WALLET_ERR_UNKNOWN;
}

walletErr_e walletbalance::receive(QString privateKey, bool *newTransactions) {
    connection_t connection = rpcapi::getConnection();
    QString accountId = signatures::getAccountIdFromPrivateKey(privateKey);
    int id = 0;
    QString response = connection->sendMessage(&id, "Receive",
                                                  QStringList({accountId}));
    if(response.length() == 0)
        return walletErr_e::WALLET_ERR_TIMEOUT;
    *newTransactions = false;
    int idRec = 1;
    while(1) {
        QApplication::processEvents();
        if(response.length() == 0)
            return walletErr_e::WALLET_ERR_UNKNOWN;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        //QJsonObject result = jsonObject["result"].toObject();
        QJsonObject jsonObjectErr = jsonObject["error"].toObject();
        if(jsonObjectErr.count() > 0)
            return WALLET_ERR_SIGNATURE;
        if(jsonObject.contains("result") && id == jsonObject["id"].toInt()) {
            return walletErr_e::WALLET_ERR_OK;
        }

        idRec = jsonObject["id"].toInt();
        QString signRec = jsonObject["method"].toString();
        qDebug() << "WALLETBALANCE 1: " << signRec;
        if(!signRec.compare("Sign")) {
            QJsonArray jsonArray = jsonObject["params"].toArray();
            if(jsonArray.count() != 0) {
                QString signature;
                if(!jsonArray[0].toString().compare("hash")) {
                    signature = signatures::getSignature(privateKey, jsonArray[1].toString(), accountId);
                } else {
                    return walletErr_e::WALLET_ERR_NOT_SUPPORTED;
                }
                QJsonArray array;
                array.append("p1393");
                array.append(signature);

                QJsonObject object;
                object.insert("jsonrpc", "2.0");
                object.insert("id", idRec);
                object.insert("result", array);
                QJsonDocument doc(object);
                QString sendMsg = QString((doc.toJson(QJsonDocument::Compact)));
                response = connection->sendSimpleMessage(sendMsg);
                *newTransactions = true;
            }
        }
    }
    return walletErr_e::WALLET_ERR_TIMEOUT;
}

walletErr_e walletbalance::send(QString privateKey, QString destAccId, QString token, double amount) {
    connection_t connection = rpcapi::getConnection();
    QString accountId = signatures::getAccountIdFromPrivateKey(privateKey);
    int id = 0;
    QString response = connection->sendMessage(&id, "Send",
                                                  QStringList({accountId, QString::asprintf("%.8f", amount), destAccId, token}));
    if(response.length() == 0)
        return walletErr_e::WALLET_ERR_TIMEOUT;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonObject result = jsonObject["result"].toObject();
    int idRec = jsonObject["id"].toInt();
    QString signRec = jsonObject["method"].toString();
    qDebug() << "WALLETBALANCE 2: " << signRec;
    if(!signRec.compare("Sign")) {
        QJsonArray jsonArray = jsonObject["params"].toArray();
        if(jsonArray.count() != 0) {
            QString signature;
            if(!jsonArray[0].toString().compare("hash")) {
                signature = signatures::getSignature(privateKey, jsonArray[1].toString(), accountId);
            } else {
                return walletErr_e::WALLET_ERR_NOT_SUPPORTED;
            }
            QJsonArray array;
            array.append("p1393");
            array.append(signature);

            QJsonObject object;
            object.insert("jsonrpc", "2.0");
            object.insert("id", idRec);
            object.insert("result", array);
            QJsonDocument doc(object);
            QString sendMsg = QString((doc.toJson(QJsonDocument::Compact)));
            response = connection->sendSimpleMessage(sendMsg);
            if(response.length() == 0)
                return walletErr_e::WALLET_ERR_UNKNOWN;
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
            QJsonObject jsonObject = jsonResponse.object();
            //QJsonObject result = jsonObject["result"].toObject();
            if(jsonObject.contains("result") && id == jsonObject["id"].toInt()) {
                return walletErr_e::WALLET_ERR_OK;
            }
        }
    }
    return walletErr_e::WALLET_ERR_UNKNOWN;
}
