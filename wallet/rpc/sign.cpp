#include "sign.h"

#include "network/rpcapi.h"
#include "crypto/signatures.h"

QJsonObject sign::signMessage(QString ownerPKey, QString response, int id) {
    connection_t connection = rpcapi::getConnection();
    QString accountId = signatures::getAccountIdFromPrivateKey(ownerPKey);
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonObject result = jsonObject["result"].toObject();
    int idRec = jsonObject["id"].toInt();
    QString signRec = jsonObject["method"].toString();
    qDebug() << "SIGN 1: " << signRec;
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
            if(jsonObject.contains("result") && id == jsonObject["id"].toInt()) {
                return jsonResponse.object();
            } else {
                return jsonObject;
            }
        }
    }
    return QJsonObject();
}
