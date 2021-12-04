#ifndef SIGN_H
#define SIGN_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "wallet/events.h"
#include "network/rpcapi.h"
#include "wallet/rpc/wallet.h"

class sign
{
public:
    typedef struct {
        walletErr_e err;
        QJsonObject result;
    }signReturn_t;

    static signReturn_t signMessage(int accNr, QString response, int id);
};

#endif // SIGN_H
