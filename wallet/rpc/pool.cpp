#include "pool.h"

#include "network/rpcapi.h"
#include "crypto/signatures.h"
#include "sign.h"
#include "wallet/events.h"

QString pool::poolInfo(QString tiken0, QString token1) {
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    return connection->sendMessage(&id, "Pool",
                                                  QStringList({tiken0, token1}));
}

walletErr_e pool::createPool(int accNr, QString token0, QString token1) {
    QString accPKey = events::getAccountKey(accNr, true, false);
    QString accId = events::getAccountId(accNr);
    if(!accPKey.length() || !accId.length())
        return walletErr_e::WALLET_ERR_UNKNOWN;
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "CreatePool",
                                                  QStringList({accId, token0, token1}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(accPKey, response, id);
    if(result.contains("balance"))
        return walletErr_e::WALLET_ERR_OK;
    return walletErr_e::WALLET_ERR_UNKNOWN;
}

walletErr_e pool::addLiquidity(int accNr, QString token0, double token0Amount, QString token1, double token1Amount) {
    QString accPKey = events::getAccountKey(accNr, true, false);
    QString accId = events::getAccountId(accNr);
    if(!accPKey.length() || !accId.length())
        return walletErr_e::WALLET_ERR_UNKNOWN;
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "AddLiquidaty",
                                                  QStringList({accId, token0, QString::asprintf("%.8f", token0Amount), token1, QString::asprintf("%.8f", token1Amount)}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(accPKey, response, id);
    if(result.contains("balance"))
        return walletErr_e::WALLET_ERR_OK;
    return walletErr_e::WALLET_ERR_UNKNOWN;
}

QString pool::poolCalculate(QString poolId, QString swapFrom, double amount, double slippage) {
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    return connection->sendMessage(&id, "PoolCalculate",
                                                  QStringList({poolId, swapFrom, QString::asprintf("%.8f", amount), QString::asprintf("%.8f", slippage)}));
}

walletErr_e pool::removeLiquidity(int accNr, QString token0, QString token1) {
    QString accPKey = events::getAccountKey(accNr, true, false);
    QString accId = events::getAccountId(accNr);
    if(!accPKey.length() || !accId.length())
        return walletErr_e::WALLET_ERR_UNKNOWN;
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "RemoveLiquidaty",
                                                  QStringList({accId, token0, token1}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(accPKey, response, id);
    if(result.contains("balance"))
        return walletErr_e::WALLET_ERR_OK;
    return walletErr_e::WALLET_ERR_UNKNOWN;
}
