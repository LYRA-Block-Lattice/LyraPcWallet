#ifndef SWAP_H
#define SWAP_H

#include "wallet.h"

class swap {
public:
    static walletErr_e swapTokens(int accNr, QString token0, QString token1, QString tokenToSwap, double amount, double amountToGet);
};

#endif // SWAP_H
