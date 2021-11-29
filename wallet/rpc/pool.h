#ifndef POOL_H
#define POOL_H

#include <QJsonObject>

#include "wallet.h"

class pool {
public:
    static QString poolInfo(QString tiken0, QString token1);
    static walletErr_e createPool(int accNr, QString token0, QString token1);
    static walletErr_e addLiquidity(int accNr, QString token0, double token0Amount, QString token1, double token1Amount);
    static QString poolCalculate(QString poolId, QString swapFrom, double amount, double slippage);
    static walletErr_e removeLiquidity(int accNr, QString token0, QString token1);
};

#endif // POOL_H
