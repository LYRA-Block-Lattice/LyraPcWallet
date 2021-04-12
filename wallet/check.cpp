#include "check.h"

#include <QRegularExpression>

#include "crypto/base58Encoding.h"

bool check::name(QString name) {
    QRegularExpression hexMatcher("^[a-zA-Z0-9_-]*$", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = hexMatcher.match(name);
    if (!match.hasMatch() || name.length() < 1) {
        return false;
    } else {
        return true;
    }
}

bool check::privateKey(QString privateKey) {
    if (base58Encoding::decodePrivateKey(privateKey).count() == 32) {
        return true;
    } else {
        return false;
    }
}

bool check::accountId(QString accountId) {
    if (base58Encoding::decodeAccountId(accountId).count() == 64) {
        return true;
    } else {
        return false;
    }
}

bool check::password(QString password1, QString password2) {
    return password1.compare(password2) == 0 && password1.length() >= 8 && password2.length() >= 8 ? true: false;
}

bool check::password(QString password) {
    return password.length() >= 8 ? true: false;
}


