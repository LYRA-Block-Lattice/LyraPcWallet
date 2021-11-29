#include "swap.h"

#include "network/rpcapi.h"
#include "crypto/signatures.h"
#include "sign.h"
#include "wallet/events.h"

walletErr_e swap::swapTokens(int accNr, QString from, double qty, QString to, double minimumReceived) {
    QString accPKey = events::getAccountKey(accNr, true, false);
    QString accId = events::getAccountId(accNr);
    if(!accPKey.length() || !accId.length())
        return walletErr_e::WALLET_ERR_UNKNOWN;
    connection_t connection = rpcapi::getConnection();
    int id = 0;
    QString response = connection->sendMessage(&id, "Swap",
                                                  QStringList({accId, "LYR", to, from, QString::asprintf("%.8f", qty), QString::asprintf("%.8f", minimumReceived)}));
    if(response.length() == 0) {
        return  walletErr_e::WALLET_ERR_TIMEOUT;
    }

    QJsonObject result = sign::signMessage(accPKey, response, id);
    if(result.contains("balance"))
        return walletErr_e::WALLET_ERR_OK;
    return walletErr_e::WALLET_ERR_UNKNOWN;
}
