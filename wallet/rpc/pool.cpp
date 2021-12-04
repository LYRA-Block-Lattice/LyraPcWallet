#include "pool.h"

#include "network/rpcapi.h"
#include "crypto/signatures.h"
#include "sign.h"
#include "wallet/events.h"

pool::poolInfo_t pool::info(QString tiker0, QString toker1) {
    pool::poolInfo_t rsp = {"", 0, 0.0, 0.0, false};
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "Pool",
                                                  QStringList({tiker0, toker1}));
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    int idRec = jsonObject["id"].toInt();
    QJsonObject result = jsonObject["result"].toObject();
    if(id == idRec && result.contains("poolId")) {
        QJsonObject balance = result["balance"].toObject();
        rsp.poolId = result["poolId"].toString();
        rsp.height = result["height"].toInt();
        rsp.amount0 = balance[tiker0].toDouble();
        rsp.amount1 = balance[toker1].toDouble();
        rsp.valid = true;
    }
    return rsp;
}

walletErr_e pool::create(int accNr, QString toker0, QString toker1) {
    QString accId = events::getAccountId(accNr);
    if(!accId.length())
        return walletErr_e::WALLET_ERR_UNKNOWN;
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "CreatePool",
                                                  QStringList({accId, toker0, toker1}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(accNr, response, id).result;
    if(result.contains("balance"))
        return walletErr_e::WALLET_ERR_OK;
    return walletErr_e::WALLET_ERR_UNKNOWN;
}

walletErr_e pool::addLiquidity(int accNr, QString toker0, double toker0Amount, QString toker1, double toker1Amount) {
    QString accId = events::getAccountId(accNr);
    if(!accId.length())
        return walletErr_e::WALLET_ERR_UNKNOWN;
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "AddLiquidaty",
                                                  QStringList({accId, toker0, QString::asprintf("%.8f", toker0Amount), toker1, QString::asprintf("%.8f", toker1Amount)}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(accNr, response, id).result;
    if(result.contains("balance"))
        return walletErr_e::WALLET_ERR_OK;
    return walletErr_e::WALLET_ERR_UNKNOWN;
}

pool::poolCalculate pool::calculate(QString poolId, QString swapFrom, double amount, double slippage) {
    poolCalculate rsp = {0.0, 0.0, "", 0.0, "", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, false};
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "PoolCalculate",
                                                  QStringList({poolId, swapFrom, QString::asprintf("%.8f", amount), QString::asprintf("%.8f", slippage)}));
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    int idRec = jsonObject["id"].toInt();
    QJsonObject result = jsonObject["result"].toObject();
    if(id == idRec && result.contains("SwapInToken")) {
        rsp.providerFee = result["ProviderFee"].toDouble();
        rsp.protocolFee = result["ProtocolFee"].toDouble();
        rsp.swapInToken = result["SwapInToken"].toString();
        rsp.swapInAmount = result["SwapInAmount"].toDouble();
        rsp.swapOutToken = result["SwapOutToken"].toString();
        rsp.swapOutAmount = result["SwapOutAmount"].toDouble();
        rsp.price = result["Price"].toDouble();
        rsp.priceImpact = result["PriceImpact"].toDouble();
        rsp.minimumReceive = result["MinimumReceived"].toDouble();
        rsp.payToProvider = result["PayToProvider"].toDouble();
        rsp.payToAuthorizer = result["PayToAuthorizer"].toDouble();
        rsp.valid = true;
    }
    return rsp;
}

walletErr_e pool::removeLiquidity(int accNr, QString toker0, QString toker1) {
    QString accId = events::getAccountId(accNr);
    if(!accId.length())
        return walletErr_e::WALLET_ERR_UNKNOWN;
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "RemoveLiquidaty",
                                                  QStringList({accId, toker0, toker1}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(accNr, response, id).result;
    if(result.contains("balance"))
        return walletErr_e::WALLET_ERR_OK;
    return walletErr_e::WALLET_ERR_UNKNOWN;
}
