#ifndef WALLETBALANCE_H
#define WALLETBALANCE_H

#include <QMap>
#include <QStringList>

#include "wallet.h"

class walletbalance
{
public:
    static ballance_t balance(QString accountId, int *height, bool *unreceived);
    static walletErr_e monitor(QString accountId, bool *unreceived);
    static walletErr_e receive(int accNr, bool *newTransactions);
    static walletErr_e send(int accNr, QString destAccId, QString token, double amount);
};

#endif // WALLETBALANCE_H
