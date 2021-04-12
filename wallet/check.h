#ifndef CHECK_H
#define CHECK_H

#include <QString>

class check
{
public:
    static bool name(QString name);
    static bool privateKey(QString privateKey);
    static bool accountId(QString accountId);
    static bool password(QString password1, QString password2);
    static bool password(QString password);
};

#endif // CHECK_H
