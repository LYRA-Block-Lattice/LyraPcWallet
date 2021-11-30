#ifndef POOL_H
#define POOL_H

#include <QJsonObject>

#include "wallet.h"

class pool {
public:
    typedef struct {
        QString poolId;
        int height;
        double amount0;
        double amount1;
        bool valid;
    }poolInfo_t;
    typedef struct {
        double providerFee;
        double protocolFee;
        QString swapInToken;
        double swapInAmount;
        QString swapOutToken;
        double swapOutAmount;
        double price;
        double priceImpact;
        double minimumReceive;
        double payToProvider;
        double payToAuthorizer;
        bool valid;
    }poolCalculate;
    static poolInfo_t info(QString tiker0, QString toker1);
    static walletErr_e create(int accNr, QString toker0, QString toker1);
    static walletErr_e addLiquidity(int accNr, QString toker0, double token0Amount, QString toker1, double token1Amount);
    static poolCalculate calculate(QString poolId, QString swapFrom, double amount, double slippage);
    static walletErr_e removeLiquidity(int accNr, QString toker0, QString toker1);
};

#endif // POOL_H
