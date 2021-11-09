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
    static walletErr_e createProfitingAcc(QString ownerPKey, QString accName, QString accType, double shareRatio, int maxVoter);
    static walletErr_e createStakingAcc(QString ownerPKey, QString accName, QString voteFor, int daysToStake, bool compoundMode);
    static walletErr_e addStaking(QString ownerPKey, QString stakingId, double amount);
    static walletErr_e unStaking(QString ownerPKey, QString stakingId);
    static walletErr_e createDividents(QString ownerPKey, QString profitingId);
    static QString getBrokerAccounts(QString accId);
};

#endif // PROFITING_H
