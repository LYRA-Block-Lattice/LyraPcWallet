
#include "walletbalance.h"
#include "network/rpcapi.h"
#include "crypto/signatures.h"
#include "sign.h"
#include "wallet/events.h"

#include "profiting.h"

walletErr_e profiting::createProfitingAcc(int accNr, QString accName, QString accType, double shareRatio, int maxVoter) {
    QString accId = events::getAccountId(accNr);
    if(!accId.length())
        return walletErr_e::WALLET_ERR_UNKNOWN;
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "CreateProfitingAccount",
                                                  QStringList({accId, accName, accType, QString::asprintf("%f", shareRatio / 100.0), QString::number(maxVoter)}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(accNr, response, id).result;
    if(result["result"].toObject().contains("pftid")) {
        return  walletErr_e::WALLET_ERR_OK;
    } else {
        QJsonObject jsonObject = result["error"].toObject();
        QString message = jsonObject["message"].toString();
        if(message.contains("SendTransactionValidationFailed")) {
            return walletErr_e::WALLET_ERR_NO_FUNDS;
        } else if(message.contains("DuplicatedName")){
            return walletErr_e::WALLET_ERR_DUPLICATED_NAME;
        } else if(message.contains("InvalidBlockTags")){
            return walletErr_e::WALLET_ERR_INVALID_BLOCK_TAGS;
        } else if(message.contains("InvalidAuthorizerCount")){
            return walletErr_e::WALLET_ERR_INVALID_AUTHORIZER_COUNT;
        } else if(message.contains("ConsensusTimeout")){
            return walletErr_e::WALLET_ERR_CONSENSUS_TIMEOUT;
        } else {
            return walletErr_e::WALLET_ERR_UNKNOWN;
        }
    }
}

walletErr_e profiting::createStakingAcc(int accNr, QString accName, QString voteFor, int daysToStake, bool compoundMode) {
    QString accId = events::getAccountId(accNr);
    if(!accId.length())
        return walletErr_e::WALLET_ERR_UNKNOWN;
    connection_t connection = rpcapi::getConnection();
    //QString accountId = signatures::getAccountIdFromPrivateKey(ownerPKey);
    int id = 0;
    QString response = connection->sendMessage(&id, "CreateStakingAccount",
                                                  QStringList({accId, accName, voteFor, QString::number(daysToStake), compoundMode ? "True" : "False"}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(accNr, response, id).result;
    if(result["result"].toObject().contains("stkid") && !voteFor.compare(result["result"].toObject()["voting"].toString())) {
        return  walletErr_e::WALLET_ERR_OK;
    } else {
        QJsonObject jsonObject = result["error"].toObject();
        QString message = jsonObject["message"].toString();
        if(message.contains("SendTransactionValidationFailed")) {
            return walletErr_e::WALLET_ERR_NO_FUNDS;
        } else if(message.contains("DuplicatedName")){
            return walletErr_e::WALLET_ERR_DUPLICATED_NAME;
        } else if(message.contains("InvalidBlockTags")){
            return walletErr_e::WALLET_ERR_INVALID_BLOCK_TAGS;
        } else if(message.contains("InvalidProfitingAccount")){
            return walletErr_e::WALLET_ERR_INVALID_PROFITING_ACCOUNT;
        } else if(message.contains("ConsensusTimeout")){
            return walletErr_e::WALLET_ERR_CONSENSUS_TIMEOUT;
        } else {
            return walletErr_e::WALLET_ERR_UNKNOWN;
        }
    }
}

walletErr_e profiting::addStaking(int accNr, QString stakingId, double amount) {
    QString accId = events::getAccountId(accNr);
    if(!accId.length())
        return walletErr_e::WALLET_ERR_UNKNOWN;
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "AddStaking",
                                                  QStringList({accId, stakingId, QString::asprintf("%f", amount)}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(accNr, response, id).result;
    result = result["result"].toObject();
    if(result["success"].toBool()) {
        return  walletErr_e::WALLET_ERR_OK;
    }
    return  walletErr_e::WALLET_ERR_UNKNOWN;
}

walletErr_e profiting::unStaking(int accNr, QString stakingId) {
    QString accId = events::getAccountId(accNr);
    if(!accId.length())
        return walletErr_e::WALLET_ERR_UNKNOWN;
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "UnStaking",
                                                  QStringList({accId, stakingId}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(accNr, response, id).result;
    result = result["result"].toObject();
    if(result["success"].toBool()) {
        return  walletErr_e::WALLET_ERR_OK;
    }
    return  walletErr_e::WALLET_ERR_UNKNOWN;
}

walletErr_e profiting::createDividents(int accNr, QString profitingId) {
    QString accId = events::getAccountId(accNr);
    if(!accId.length())
        return walletErr_e::WALLET_ERR_UNKNOWN;
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "CreateDividends",
                                                  QStringList({accId, profitingId}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(accNr, response, id).result;
    result = result["result"].toObject();
    if(result["success"].toBool()) {
        return  walletErr_e::WALLET_ERR_OK;
    }
    return  walletErr_e::WALLET_ERR_UNKNOWN;
}

QString profiting::getBrokerAccounts(int accNr) {
    QString accId = events::getAccountId(accNr);
    if(!accId.length())
        return "";
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    return connection->sendMessage(&id, "GetBrokerAccounts",
                                                  QStringList({accId}));
}

QString profiting::getPendingStats(int accNr) {
    QString accId = events::getAccountId(accNr);
    if(!accId.length())
        return "";
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    return connection->sendMessage(&id, "GetPendingStats",
                                                  QStringList({accId}));
}
