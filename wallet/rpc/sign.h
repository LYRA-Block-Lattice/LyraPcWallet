#ifndef SIGN_H
#define SIGN_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class sign
{
public:
    static QJsonObject signMessage(QString ownerPKey, QString response, int id);
};

#endif // SIGN_H
