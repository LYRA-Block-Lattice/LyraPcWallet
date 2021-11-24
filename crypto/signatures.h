#ifndef SIGNATURES_H
#define SIGNATURES_H

#include <QString>

class signatures
{
public:
    typedef struct {
        QString privateKey;
        QString accountId;
    }generatedAccount;

    static QString getAccountIdFromPrivateKey(QString privKey);
    static generatedAccount generateAccount(QByteArray key);
    static generatedAccount generateAccount();
    static bool validateAccountId(QString id);
    static bool validatePublicKey(QString pubKey);
    static bool validatePrivateKey(QString privKey);
    static bool verifyAccountSignature(QString message, QString accountId, QString signature);
    static bool verifySignature(QString message, QString accountId, QString signature);
    static QString getSignature(QString privateKey, QString message, QString accountId);
};

#endif // SIGNATURES_H
