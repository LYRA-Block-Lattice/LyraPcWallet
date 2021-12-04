#include "swap.h"

#include "network/rpcapi.h"
#include "crypto/signatures.h"
#include "sign.h"
#include "wallet/events.h"

walletErr_e swap::swapTokens(int accNr, QString token0, QString token1, QString tokenToSwap, double amount, double amountToGet) {
    QString accId = events::getAccountId(accNr);
    if(!accId.length())
        return walletErr_e::WALLET_ERR_UNKNOWN;
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "Swap",
                                                  QStringList({accId, token0, token1, tokenToSwap, QString::asprintf("%.8f", amount), QString::asprintf("%.8f", amountToGet)}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(accNr, response, id).result;
    if(result.contains("result") && result["result"].toObject().contains("balance"))
        return walletErr_e::WALLET_ERR_OK;
    return walletErr_e::WALLET_ERR_UNKNOWN;
}
