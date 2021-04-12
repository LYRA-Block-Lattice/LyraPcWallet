#ifndef BASE58_H
#define BASE58_H

#include <QByteArray>
#include <QString>


class base58Encoding {
public:
    static QString encode(QByteArray data);
    static QByteArray decode(QString str);
    static QByteArray getCheckSum(QByteArray data);
    static QByteArray verifyAndRemoveCheckSum(QByteArray data);
    static QByteArray addCheckSum(QByteArray data);
    static QString encodeWithCheckSum(QByteArray data);
    static QString encodePrivateKey(QByteArray privKey);
    static QString encodePublicKey(QByteArray pubKey);
    static QString encodeAccountId(QByteArray pubKey);
    static QByteArray decodeWithCheckSum(QString data);
    static QByteArray decodePrivateKey(QString privKey);
    static QByteArray decodePublicKey(QString pubKey);
    static QByteArray decodeAccountId(QString accountId);
};

#endif // BASE58_H
