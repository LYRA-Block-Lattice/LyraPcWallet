#include <QRandomGenerator>
#include <QCryptographicHash>

#include "signatures.h"
#include "base58Encoding.h"
#include "ecc.h"
#include "configlyra.h"

QString signatures::getAccountIdFromPrivateKey(QString privKey) {
    QByteArray prKey = base58Encoding::decodePrivateKey(privKey);
    EccPoint pubKey;
    ecc::make_key(&pubKey, (uint8_t *)prKey.data(), (uint8_t *)prKey.data());
    QByteArray puKey;
    puKey.append((char *)&pubKey.x, 32);
    puKey.append((char *)&pubKey.y, 32);
    return base58Encoding::encodeAccountId(puKey);
}

signatures::generatedWallet signatures::generateWallet(QByteArray key) {
    signatures::generatedWallet acc;
    acc.accountId = getAccountIdFromPrivateKey(key);
    acc.privateKey = base58Encoding::encodePrivateKey(key);
    return acc;
}

signatures::generatedWallet signatures::generateWallet() {
    quint32 array[8];
    QRandomGenerator::global()->fillRange(array);
    QByteArray key;
    for(quint32 tmp: array) {
        key.append((tmp >> 24) & 0xFF);
        key.append((tmp >> 16) & 0xFF);
        key.append((tmp >> 8) & 0xFF);
        key.append(tmp & 0xFF);
    }
    return generateWallet(key);
}

bool signatures::validateAccountId(QString id) {
    if(base58Encoding::decodeAccountId(id) == nullptr || id.at(0) != LYRA_ADDRESSPREFIX)
        return false;
    else
        return true;
}

bool signatures::validatePublicKey(QString pubKey) {
    return base58Encoding::decodePublicKey(pubKey) != nullptr;
}

bool signatures::validatePrivateKey(QString privKey) {
    return base58Encoding::decodePrivateKey(privKey) != nullptr;
}

bool signatures::verifyAccountSignature(QString message, QString accountId, QString signature) {
    return verifySignature(message, accountId, signature);
}

bool signatures::verifySignature(QString message, QString accountId, QString signature) {
    QByteArray signatureBytes = base58Encoding::decode(signature);
    QByteArray publicKeyBytes = base58Encoding::decodeAccountId(accountId);
    EccPoint pubKey;
    EccPoint sig;
    int cnt = 0;
    for(; cnt < 32; cnt++) {
        pubKey.x[cnt] = publicKeyBytes[cnt];
        pubKey.y[cnt] = publicKeyBytes[cnt + 32];
        sig.x[cnt] = signatureBytes[cnt];
        sig.y[cnt] = signatureBytes[cnt + 32];
    }

    QCryptographicHash sha256 (QCryptographicHash::Sha256);
    sha256.reset();
    sha256.addData(message.toUtf8().data(), message.length());
    QByteArray hash = sha256 .result();
    return ecc::ecdsa_verify(&pubKey, (uint8_t *)hash.data(), sig.x, sig.y);
}

QString signatures::getSignature(QString privateKey, QString message, QString /*accountId*/) {
    QByteArray privateKeyBytes = base58Encoding::decodePrivateKey(privateKey);
    //QByteArray publicKeyBytes = base58Encoding::decodeAccountId(accountId);
    uint8_t pKey[NUM_ECC_DIGITS];
    int cnt = 0;
    for(; cnt < 32; cnt++) {
        pKey[cnt] = privateKeyBytes[cnt];
    }

    QCryptographicHash sha256 (QCryptographicHash::Sha256);
    sha256.reset();
    sha256.addData(message.toUtf8(), message.length());
    QByteArray hash = sha256 .result();
    uint8_t sig[NUM_ECC_DIGITS * 2];
    ecc::ecdsa_sign(pKey, (uint8_t *)hash.data(), sig);
    return base58Encoding::encode(QByteArray((char *)sig, 64));
}




