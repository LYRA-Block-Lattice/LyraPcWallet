#ifndef TEXTFORMATING_H
#define TEXTFORMATING_H

#include <QString>

class textformating
{
public:
    static QString toValue(double sum = 0, int precision = 2, bool noComma = false);
    static QString toValue(QString value, int precision = 8);
    static QString toPercentage(double sum);
    static QString toTranslatedMonth(int month);
    static QString toDate(qint64 date);
    static QString toTime(qint64 date);
};

#endif // TEXTFORMATING_H
