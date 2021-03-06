#ifndef WALLET_H
#define WALLET_H

#include <QString>
#include <QMap>

typedef QMap<QString, double> ballance_t;
typedef QList<QMap<QString, QString>> transaction_t;

typedef enum {
    WALLET_ERR_OK = 0,
    WALLET_ERR_UNKNOWN,
    WALLET_ERR_NOT_SUPPORTED,
    WALLET_ERR_CONNECTION_CLOSED,
    WALLET_ERR_TIMEOUT,
    WALLET_ERR_SIGNATURE,
    WALLET_ERR_ACC_MISSMATCH,
    WALLET_ERR_DUPLICATED_NAME,
    WALLET_ERR_NO_FUNDS,
    WALLET_ERR_INVALID_BLOCK_TAGS,
    WALLET_ERR_INVALID_PROFITING_ACCOUNT,
    WALLET_ERR_INVALID_AUTHORIZER_COUNT,
    WALLET_ERR_CONSENSUS_TIMEOUT
}walletErr_e;

class wallet
{
public:
    static void sync();
    static void checkNewTransactions();
    static void calculateLastWeek();
};

#endif // WALLET_H
