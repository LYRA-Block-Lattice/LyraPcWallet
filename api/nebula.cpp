#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "nebula.h"

#include "network/tokenpairing.h"
#include "wallet/events.h"

bool nebula::getCirculation(double *teamTotal, double *circulate, double *burned) {
    QString response = tokenpairing::getStatic((events::getNetwork() == events::network_e::NETWORK_MAINNET) ? "https://nebula.lyra.live/api/nebula/supply" : "https://blockexplorer.testnet.lyra.live/api/nebula/supply");
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
    QString response = tokenpairing::getStatic((events::getNetwork() == events::network_e::NETWORK_MAINNET) ? "https://nebula.lyra.live/api/nebula/history" : "https://blockexplorer.testnet.lyra.live/api/nebula/history");
    if(response == "") {
        return false;
    }
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonArray jsonArray = jsonResponse.array();
    int count = jsonArray.count();
    if (count) {
        QJsonValue value = jsonArray[count - 1];
        QJsonObject nodeStatus = value["nodeStatus"].toObject();
        foreach(const QJsonValue &node, nodeStatus) {
            QJsonObject nodeStatus = node["status"].toObject();
            *height = nodeStatus["totalBlockCount"].toVariant().toLongLong();
            if(*height) {
                return true;
            } else {
                return false;
            }
        }

    }
    return true;
}
