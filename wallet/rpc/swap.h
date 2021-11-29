#ifndef SWAP_H
#define SWAP_H

#include "wallet.h"

class swap {
public:
    static walletErr_e swapTokens(int accNr, QString from, double qty, QString to, double minimumReceived);
};

#endif // SWAP_H
