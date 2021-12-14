#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QApplication>

#include "walletbalance.h"
#include "network/rpcapi.h"
#include "crypto/signatures.h"
#include "wallet/events.h"

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
        if(events::getAppClosing())
            break;
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

walletErr_e walletbalance::receive(int accNr, bool *newTransactions) {
    QString accPKey = events::getAccountKey(accNr, false, true);
    QString accId = events::getAccountId(accNr);
    if(!accPKey.length() || !accId.length())
        return walletErr_e::WALLET_ERR_UNKNOWN;
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "Receive",
                                                  QStringList({accId}));
    if(response.length() == 0)
        return walletErr_e::WALLET_ERR_TIMEOUT;
    *newTransactions = false;
    int idRec = 1;
    while(1) {
#if VORBOSE_LEVEL >= 5
        qDebug() << "WALLETBALANCE 5 :Loop ";
#endif
        if(events::getAppClosing())
            break;
        QApplication::processEvents();
        if(!response.length()) {
            qDebug() << "WALLETBALANCE 1: No response";
            return walletErr_e::WALLET_ERR_UNKNOWN;
        }
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        //QJsonObject result = jsonObject["result"].toObject();
        QJsonObject jsonObjectErr = jsonObject["error"].toObject();
        if(jsonObjectErr.count() > 0)
            return WALLET_ERR_SIGNATURE;
        if(jsonObject.contains("result") && id == jsonObject["id"].toInt()) {
            events::setTriggerNodeFetch();
            return walletErr_e::WALLET_ERR_OK;
        }
        idRec = jsonObject["id"].toInt();
        QString signRec = jsonObject["method"].toString();
        qDebug() << "WALLETBALANCE 2: " << signRec;
        if(!signRec.length())
            break;
        if(!signRec.compare("Sign")) {
            QJsonArray jsonArray = jsonObject["params"].toArray();
            if(jsonArray.count() != 0) {
                QString signature;
                if(!jsonArray[0].toString().compare("hash"))
                    signature = signatures::getSignature(accPKey, jsonArray[1].toString(), accId);
                else
                    return walletErr_e::WALLET_ERR_NOT_SUPPORTED;
                QJsonArray array;
                array.append("p1393");
                array.append(signature);
                QJsonObject object;
                object.insert("jsonrpc", "2.0");
                object.insert("id", idRec);
                object.insert("result", array);
                QJsonDocument doc(object);
                QString sendMsg = QString((doc.toJson(QJsonDocument::Compact)));
                qDebug() << "WALLETBALANCE 4: " << accNr << sendMsg;
                response = connection->sendSimpleMessage(sendMsg);
                qDebug() << "WALLETBALANCE 5: " << accNr << response;
                *newTransactions = true;
            }
        }
    }
    return walletErr_e::WALLET_ERR_TIMEOUT;
}

walletErr_e walletbalance::send(int accNr, QString destAccId, QString token, double amount) {
    QString accPKey = events::getAccountKey(accNr, true, false);
    QString accId = events::getAccountId(accNr);
    if(!accPKey.length() || !accId.length())
        return walletErr_e::WALLET_ERR_UNKNOWN;
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "Send",
                                                  QStringList({accId, QString::asprintf("%.8f", amount), destAccId, token}));
    if(!response.length()) {
        qDebug() << "WALLETBALANCE 3: No response";
        return walletErr_e::WALLET_ERR_UNKNOWN;
    }
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonObject result = jsonObject["result"].toObject();
    int idRec = jsonObject["id"].toInt();
    QString signRec = jsonObject["method"].toString();
    qDebug() << "WALLETBALANCE 4: " << signRec;
    if(!signRec.compare("Sign")) {
        QJsonArray jsonArray = jsonObject["params"].toArray();
        if(jsonArray.count() != 0) {
            QString signature;
            if(!jsonArray[0].toString().compare("hash"))
                signature = signatures::getSignature(accPKey, jsonArray[1].toString(), accId);
            else
                return walletErr_e::WALLET_ERR_NOT_SUPPORTED;
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
                events::setTriggerNodeFetch();
                return walletErr_e::WALLET_ERR_OK;
            }
        }
    }
    return walletErr_e::WALLET_ERR_UNKNOWN;
}
