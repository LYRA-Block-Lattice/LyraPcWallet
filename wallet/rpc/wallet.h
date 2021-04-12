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
    WALLET_ERR_SIGNATURE
}walletErr_e;

class wallet
{
public:
    static void sync();
    static void calculateLastWeek();
};

#endif // WALLET_H
