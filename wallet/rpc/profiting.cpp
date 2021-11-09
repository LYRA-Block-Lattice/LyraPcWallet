
#include "walletbalance.h"
#include "network/rpcapi.h"
#include "crypto/signatures.h"
#include "sign.h"

#include "profiting.h"

walletErr_e profiting::createProfitingAcc(QString ownerPKey, QString accName, QString accType, double shareRatio, int maxVoter) {
    connection_t connection = rpcapi::getConnection();
    QString accountId = signatures::getAccountIdFromPrivateKey(ownerPKey);
    int id = 0;
    QString response = connection->sendMessage(&id, "CreateProfitingAccount",
                                                  QStringList({accountId, accName, accType, QString::asprintf("%f", shareRatio / 100.0), QString::number(maxVoter)}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(ownerPKey, response, id);
    if(result.contains("pftid")) {
        return  walletErr_e::WALLET_ERR_OK;
    }
    return  walletErr_e::WALLET_ERR_UNKNOWN;
}

walletErr_e profiting::createStakingAcc(QString ownerPKey, QString accName, QString voteFor, int daysToStake, bool compoundMode) {
    connection_t connection = rpcapi::getConnection();
    QString accountId = signatures::getAccountIdFromPrivateKey(ownerPKey);
    int id = 0;
    QString response = connection->sendMessage(&id, "CreateStakingAccount",
                                                  QStringList({accountId, accName, voteFor, QString::number(daysToStake), compoundMode ? "True" : "False"}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(ownerPKey, response, id);
    if(result.contains("stkid") && !voteFor.compare(result["voting"].toString())) {
        return  walletErr_e::WALLET_ERR_OK;
    }
    return  walletErr_e::WALLET_ERR_UNKNOWN;
}

walletErr_e profiting::addStaking(QString ownerPKey, QString stakingId, double amount) {
    connection_t connection = rpcapi::getConnection();
    QString accountId = signatures::getAccountIdFromPrivateKey(ownerPKey);
    int id = 0;
    QString response = connection->sendMessage(&id, "AddStaking",
                                                  QStringList({accountId, stakingId, QString::asprintf("%f", amount)}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(ownerPKey, response, id);
    if(result["success"].toBool()) {
        return  walletErr_e::WALLET_ERR_OK;
    }
    return  walletErr_e::WALLET_ERR_UNKNOWN;
}

walletErr_e profiting::unStaking(QString ownerPKey, QString stakingId) {
    connection_t connection = rpcapi::getConnection();
    QString accountId = signatures::getAccountIdFromPrivateKey(ownerPKey);
    int id = 0;
    QString response = connection->sendMessage(&id, "UnStaking",
                                                  QStringList({accountId, stakingId}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(ownerPKey, response, id);
    if(result["success"].toBool()) {
        return  walletErr_e::WALLET_ERR_OK;
    }
    return  walletErr_e::WALLET_ERR_UNKNOWN;
}

walletErr_e profiting::createDividents(QString ownerPKey, QString profitingId) {
    connection_t connection = rpcapi::getConnection();
    QString accountId = signatures::getAccountIdFromPrivateKey(ownerPKey);
    int id = 0;
    QString response = connection->sendMessage(&id, "CreateDividends",
                                                  QStringList({accountId, profitingId}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(ownerPKey, response, id);
    if(result["success"].toBool()) {
        return  walletErr_e::WALLET_ERR_OK;
    }
    return  walletErr_e::WALLET_ERR_UNKNOWN;
}

QString profiting::getBrokerAccounts(QString accId) {
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    return connection->sendMessage(&id, "GetBrokerAccounts",
                                                  QStringList({accId}));
}
