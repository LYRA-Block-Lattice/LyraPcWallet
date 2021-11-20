#ifndef PROFITING_H
#define PROFITING_H

#include <QList>
#include <QPair>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "wallet.h"

class profiting
{
public:
    static walletErr_e createProfitingAcc(int accNr, QString accName, QString accType, double shareRatio, int maxVoter);
    static walletErr_e createStakingAcc(int accNr, QString accName, QString voteFor, int daysToStake, bool compoundMode);
    static walletErr_e addStaking(int accNr, QString stakingId, double amount);
    static walletErr_e unStaking(int accNr, QString stakingId);
    static walletErr_e createDividents(int accNr, QString profitingId);
    static QString getBrokerAccounts(int accNr);
    static QString getPendingStats(int accNr);
};

#endif // PROFITING_H
