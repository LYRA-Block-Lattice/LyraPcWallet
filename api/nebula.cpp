#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "nebula.h"

#include "network/tokenpairing.h"
#include "wallet/events.h"

bool nebula::getCirculation(double *teamTotal, double *circulate, double *burned) {
    QString response = tokenpairing::getStatic((events::getNetwork() == events::network_e::NETWORK_MAINNET) ? "https://nebula.lyra.live/api/nebula/supply" : "https://nebulatestnet.lyra.live/api/nebula/supply");
    if(response == "") {
        return false;
    }
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    *teamTotal = jsonObject["teamTotal"].toDouble();
    *circulate = jsonObject["circulate"].toDouble();
    *burned = jsonObject["burned"].toDouble();

    return true;
}

bool nebula::getBlockHeight(long long *height) {
    qDebug() << "NEBULA 1 :" << ((events::getNetwork() == events::network_e::NETWORK_MAINNET) ? "https://mainnet.lyra.live/api/node/GetLastBlock" : "https://testnet.lyra.live/api/node/GetLastBlock");
    QString response = tokenpairing::getStatic((events::getNetwork() == events::network_e::NETWORK_MAINNET) ? "https://mainnet.lyra.live/api/node/GetLastBlock" : "https://testnet.lyra.live/api/node/GetLastBlock");
    if(response == "") {
        return false;
    }
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject blockDataObject = jsonResponse.object();
    QJsonValue blockDataValue = blockDataObject["blockData"].toString();

    jsonResponse = QJsonDocument::fromJson(blockDataValue.toString().toUtf8());
    blockDataObject = jsonResponse.object();
    *height = blockDataObject["totalBlockCount"].toVariant().toLongLong();
    /*if(!*height){
        return false;
    }*/
    return true;
}
