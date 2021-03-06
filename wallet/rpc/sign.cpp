#include <QApplication>

#include "sign.h"

#include "network/rpcapi.h"
#include "crypto/signatures.h"
#include "wallet/events.h"

/*QJsonObject sign::signMessage(QString ownerPKey, QString response, int id) {
    connection_t connection = rpcapi::getConnection();
    QString accountId = signatures::getAccountIdFromPrivateKey(ownerPKey);
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonObject result = jsonObject["result"].toObject();
    int idRec = jsonObject["id"].toInt();
    QString signRec = jsonObject["method"].toString();
#if VORBOSE_LEVEL >= 0
    qDebug() << "SIGN 1: " << response;
#endif
    if(!signRec.compare("Sign")) {
        QJsonArray jsonArray = jsonObject["params"].toArray();
        if(jsonArray.count() != 0) {
            if(accountId.compare(jsonArray[2].toString())) {
                return QJsonObject();
            }
            QString signature;
            if(!jsonArray[0].toString().compare("hash")) {
                signature = signatures::getSignature(ownerPKey, jsonArray[1].toString(), accountId);
            } else {
                return QJsonObject();
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
            if(response.length() == 0) {
                return QJsonObject();
            }
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
            QJsonObject jsonObject = jsonResponse.object();
#if VORBOSE_LEVEL >= 0
            qDebug() << "SIGN 2: " << jsonObject;
#endif
            if(jsonObject.contains("result") && id == jsonObject["id"].toInt()) {
                return jsonResponse.object();
            } else {
                return jsonObject;
            }
        }
    }
    return QJsonObject();
}*/

sign::signReturn_t sign::signMessage(int accNr, QString response, int id) {
    QString accPKey = events::getAccountKey(accNr, true, false);
    QString accId = events::getAccountId(accNr);
    signReturn_t result = {walletErr_e::WALLET_ERR_UNKNOWN, QJsonObject()};
    if(!accPKey.length() || !accId.length()) {
        result.err = walletErr_e::WALLET_ERR_UNKNOWN;
        return result;
    }
    if(response.length() == 0)
        return result;
    connection_t connection = rpcapi::getConnection();
    int idRec = 1;
#if VORBOSE_LEVEL >= 1
    qDebug() << "SIGN 1: " << response;
#endif
    while(1) {
        if(events::getAppClosing())
            break;
        QApplication::processEvents();
        if(!response.length()) {
            return result;
        }
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        //QJsonObject result = jsonObject["result"].toObject();
        QJsonObject jsonObjectErr = jsonObject["error"].toObject();
        if(jsonObjectErr.count() > 0) {
            result.err = WALLET_ERR_SIGNATURE;
            return result;
        }
        if(jsonObject.contains("result") && id == jsonObject["id"].toInt()) {
            events::setTriggerNodeFetch();
            result.err = walletErr_e::WALLET_ERR_OK;
            result.result = jsonResponse.object();
            return result;
        }
        idRec = jsonObject["id"].toInt();
        QString signRec = jsonObject["method"].toString();
        if(!signRec.length())
            break;
        if(!signRec.compare("Sign")) {
            QJsonArray jsonArray = jsonObject["params"].toArray();
            if(jsonArray.count() != 0) {
                QString signature;
                if(!jsonArray[0].toString().compare("hash"))
                    signature = signatures::getSignature(accPKey, jsonArray[1].toString(), accId);
                else {
                    result.err = walletErr_e::WALLET_ERR_NOT_SUPPORTED;
                    return result;
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
#if VORBOSE_LEVEL >= 1
                qDebug() << "SIGN 2: " << sendMsg;
#endif
                response = connection->sendSimpleMessage(sendMsg);
#if VORBOSE_LEVEL >= 1
                qDebug() << "SIGN 3: " << response;
#endif
            }
        }
    }
    return result;
}
